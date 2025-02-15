#include "Parser.h"

#include <cassert>
#include <iostream>
#include <set>
#include <format>
#include <stack>

namespace Compiler
{

struct FunctionArgs
{

};
	
static size_t pos = 0;
static Token* token = nullptr;
static std::vector<Token> current_tokens;
static std::vector<std::string> errors;

static size_t register_pos = 0;
static std::set<std::string> function_sigs;
static std::vector<std::unordered_map<std::string_view, VMRegister>> vars;

static std::vector<VM::Instruction> parse_res;

extern VMRegister Expression();

template<typename ...TArgs> 
static void AddError(std::string_view format, TArgs... args)
{
	char buf[128]{};
	snprintf(buf, sizeof(buf), format.data(), args...);
	std::cout << buf << std::endl;
	errors.emplace_back(buf);
}

static VMRegister CreateRegisterDst(int reg)
{
	VMRegister dst{};
	dst.value.num = reg;
	dst.type = REGISTER;
	return dst;
}

static VMRegister* GetVarRegister(const std::string& id)
{
	auto pos = vars.back().find(id);
	if (pos != vars.back().end())
		return &pos->second;

	return nullptr;
}

// temp
static void PrintInstruction(OP_CODE op_code, std::vector<VMRegister> args = {})
{
	std::string s = op_code_names[op_code] + ' ';
	for (const VMRegister& arg : args)
	{
		switch (arg.type)
		{
		case VMRegisterType::FLOAT:
			s += std::to_string(arg.value.flt);
			break;
		case VMRegisterType::INT:
			s += std::to_string(arg.value.num);
			break;
		case VMRegisterType::STRING:
			s += arg.value.str;
			break;
		case VMRegisterType::REGISTER:
			s += 'r' + std::to_string(arg.value.num);
			break;
		}
		s += ' ';
	}

	std::cout << s << std::endl;
}

static void AddInstruction(OP_CODE op_code, std::vector<VMRegister> args = {})
{
	// temp
	PrintInstruction(op_code, args);

	parse_res.emplace_back(op_code, args);
}

static void IncrementToken()
{
	pos++;
	if (pos < current_tokens.size())
		token = &current_tokens[pos];
	else
		token = nullptr;
}

static std::string RegToType(const VMRegister& reg)
{
	switch (reg.type)
	{
	case REGISTER:
		return "register";
	case INT:
		return "int";
	case STRING:
		return "string";
	case FLOAT:
		return "float";
	default:
		return "UnkownType";
	}
}

// Checks the registers and returns the comma separated types a=1;b=2; (r0, r1) = (int, int)
static std::string RegsToTypes(const std::vector<VMRegister>& args)
{
	std::string res;

	for (const VMRegister& reg : args)
		res += RegToType(reg);

	return res;
}

static Token* PeekNextToken()
{
	Token* res = nullptr;
	if (pos + 1 < current_tokens.size())
		res = &current_tokens[pos + 1];

	return res;
}

static Token* PeekPreviousToken()
{
	Token* res = nullptr;
	if (pos != 0)
		res = &current_tokens[pos - 1];

	return res;
}

static bool IsTokenKeyword(Token& token)
{
	return token.type == TOKEN_TYPE::INT || token.type == TOKEN_TYPE::FLOAT /*|| token.type == TOKEN_TYPE::STRING*/;
}

static bool IsTokenOperatorAssign(Token& token)
{
	return token.type == TOKEN_TYPE::DIVIDE_AND_ASSIGN ||
		token.type == TOKEN_TYPE::MULTIPLY_AND_ASSIGN ||
		token.type == TOKEN_TYPE::PLUS_AND_ASSIGN ||
		token.type == TOKEN_TYPE::MINUS_AND_ASSIGN ||
		token.type == TOKEN_TYPE::ASSIGNMENT;
}

extern VMRegister PlusMinus();

static VMRegister Unary()
{
	VMRegister res {};
	res.type = INT;
	res.value.num = -1;

	const bool is_unary_min = token->type == TOKEN_TYPE::MINUS;
	const bool is_not = token->type == TOKEN_TYPE::NOT;

	if (is_unary_min || is_not)
		IncrementToken();
	
	if (token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		IncrementToken();
		res = PlusMinus();
		IncrementToken();
		if (token->type == TOKEN_TYPE::PARENTHESES_RIGHT)
		{
			if (is_unary_min || is_not)
			{
				VMRegister dst = CreateRegisterDst(register_pos);
				res.value.num = register_pos;
				register_pos++;

				if (is_unary_min)
					AddInstruction(OP_NEGATE, { res });
				else if (is_not)
					AddInstruction(OP_NOT, { res });

				AddInstruction(OP_MOVE, { dst, res });
			}

			return res;
		}
		else
			AddError("Expected ')' at pos: %d", pos);
	}
	else if (token->type == TOKEN_TYPE::IDENTIFIER)
	{
		res.type = REGISTER;

		// get var register
		VMRegister* reg = GetVarRegister(token->str);
		if (!reg)
		{
			// check function signature
			AddError("Identifier not found: %s", token->str.c_str());
		}
		else
		{
			res.value.num = reg->value.num;
			//register_pos++;
			//AddInstruction(OP_MOVE, { res, *reg });

			if (is_unary_min)
				AddInstruction(OP_NEGATE, { res });
		}
	}
	else if (token->type == TOKEN_TYPE::NUMBER)
	{
		res.type = REGISTER;
		int val = std::stoi(token->str);

		if (is_unary_min)
			val = -val;

		VMRegister src{};
		src.type = INT;
		src.value.num = val;

		res.value.num = register_pos;
		register_pos++;
		AddInstruction(OP_MOVE, { res, src });

		return res;
	}
	else if (token->type == TOKEN_TYPE::FLOAT)
	{
	}

	return res;
}

static VMRegister Factor()
{
	VMRegister a = Unary();
	if (a.value.num == -1)
	{
		AddError("Unexpected token after: %s", token->str.c_str());
	}

	VMRegister dst = a;

	Token* next = PeekNextToken();
	if (next && (next->type == TOKEN_TYPE::MULTIPLY || next->type == TOKEN_TYPE::DIVIDE))
	{
		IncrementToken();

		while (next && (next->type == TOKEN_TYPE::MULTIPLY || next->type == TOKEN_TYPE::DIVIDE))
		{
			IncrementToken();
			VMRegister b = Unary();
			dst = CreateRegisterDst(register_pos);
			register_pos++;

			if (next->type == TOKEN_TYPE::MULTIPLY)
				AddInstruction(OP_MULTIPLY, { dst, a, b });
			else 
				AddInstruction(OP_DIVIDE, { dst, a, b });

			next = PeekNextToken();
			if (next && (next->type == TOKEN_TYPE::MULTIPLY || next->type == TOKEN_TYPE::DIVIDE))
			{
				a = dst;
				IncrementToken();
			}
		}
	}

	return dst;
}

static VMRegister PlusMinus()
{
	VMRegister a = Factor();
	if (a.value.num == -1)
	{
		AddError("Unexpected token after: {}", token->str);
	}

	VMRegister dst = a;

	Token* next = PeekNextToken();
	if (next && (next->type == TOKEN_TYPE::PLUS || next->type == TOKEN_TYPE::MINUS))
	{
		IncrementToken();

		while (next && (next->type == TOKEN_TYPE::PLUS || next->type == TOKEN_TYPE::MINUS))
		{
			IncrementToken();
			int rp_prev = register_pos;
			VMRegister b = Factor();
			dst = CreateRegisterDst(rp_prev);
			register_pos = rp_prev + 1;

			if (next->type == TOKEN_TYPE::PLUS)
				AddInstruction(OP_ADD, { dst, a, b });
			else 
				AddInstruction(OP_SUBTRACT, { dst, a, b });

			next = PeekNextToken();
			if (next && (next->type == TOKEN_TYPE::PLUS || next->type == TOKEN_TYPE::MINUS))
			{
				a = dst;
				IncrementToken();
			}
		}
	}

	return dst;
}

static VMRegister IfStatement()
{
	return {};
}

static VMRegister ForLoop()
{
	return {  };
}

static VMRegister Identifier()
{
	VMRegister res{};
	res.type = STRING;
	res.value.str = token->str.c_str();

	// check if this identifier is a var that already exists 
	for (size_t i = vars.size(); i-- > 0; )
	{
		auto varit = vars[i].find(res.value.str);
		if (varit != vars[i].end())
		{
			res = varit->second;

			Token* prev = PeekPreviousToken();
			if (prev)
			{
				if (IsTokenKeyword(*prev))
				{
					AddError("Variable with identifier {} already exists", token->str);
					return res;
				}
				else
				{
					// get the variable and checc
					IncrementToken();
					if (!token)
						return res;
					
					if (IsTokenOperatorAssign(*token))
					{
						IncrementToken();
						if (!token)
							return res;

						VMRegister reg = PlusMinus();
						AddInstruction(OP_MOVE, { varit->second, reg });
					}
				}
			}
			
			return res;
		}
	}

	IncrementToken();

	if (!token)
		return res;

	// check if function call or definition
	if (token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		res.type = FUNCTION;
		std::vector<VMRegister> args_as_registers{};
		std::string arg_list;

		for (size_t i = pos; i < current_tokens.size(); i++)
		{
			Token& t = current_tokens[i];

			if (t.type == TOKEN_TYPE::IDENTIFIER)
			{
				VMRegister* reg = GetVarRegister(t.str);

				// #TODO VALIDATE FOR NUM VALUEs
				if (reg)
				{
					pos = i;
					token = &current_tokens[i];
					// numerical
					VMRegister arg = PlusMinus();

					// check if -1 and in that case it probably ended the expression
					if (arg.value.num != -1)
					{
						assert(arg.type == REGISTER);
						args_as_registers.emplace_back(arg);
						i = pos;
					}
				}
			}

			if (t.type == TOKEN_TYPE::NUMBER)
			{
				pos = i;
				token = &current_tokens[i];
				// numerical
				VMRegister arg = PlusMinus();

				// check if -1 and in that case it probably ended the expression
				if (arg.value.num != -1)
				{
					assert(arg.type == REGISTER);
					args_as_registers.emplace_back(arg);
					i = pos;
				}
			}

			arg_list += t.str;

			//Register* arg_reg = GetVarRegister(t.str);
			//if (arg_reg)
			//{
			//	args_as_registers.emplace_back(*arg_reg);
			//}
		
			// end of args 
			if (t.type == TOKEN_TYPE::PARENTHESES_RIGHT)
			{
				pos = i;

				std::string sig = res.value.str;
				sig += ' ';
				sig += RegsToTypes(args_as_registers);

				Token* next = PeekNextToken();
				if (!next)
				{
					AddError("Expected token at:");
					return res;
				}

				// check if definition
				if (next->type == TOKEN_TYPE::BRACKET_LEFT)
				{
					auto [stored_sig, _] = function_sigs.emplace(sig);
					VMRegister func{};
					func.type = STRING;
					func.value.str = stored_sig->c_str();

					AddInstruction(OP_LABEL, {func});

					while (token->type != TOKEN_TYPE::BRACKET_RIGHT)
						Expression();

					AddInstruction(OP_RETURN);
				}
				else if (next->type == TOKEN_TYPE::SEMICOLON)
				{
					// check if there exists a signature that takes these argument types 
					if (!function_sigs.contains(sig))
					{
						// check if there exists a signature that takes all types 
						std::string func_name = sig.substr(0, sig.find(' '));
						if (function_sigs.contains(func_name + " ..."))
							sig = func_name + " ...";
						else
						{
							// cooked 
							AddError("No function found with signature %s", sig.c_str());
							return res;
						}
					}
				
					// #TODo: temp leak for now 
					VMRegister function_arg{};
					function_arg.type = STRING;
					void* p = malloc(sig.size());
					assert(p);
					memcpy(p, sig.c_str(), sig.size() + 1);
					function_arg.value.str = (char*)p;
					args_as_registers.insert(args_as_registers.begin(), function_arg);

					AddInstruction(OP_CALL, args_as_registers);
					break;
				}
			}
		}
	}

	return res;
}

static void IntKeyword()
{
	// check next token, should be identifier
	IncrementToken();

	if (!token)
	{
		AddError("Expected token");
		return;
	}

	if (token->type != TOKEN_TYPE::IDENTIFIER)
	{
		AddError("Unexpected identifier after int at: %d", pos);
		return;
	}

	VMRegister id_res = Identifier();
	
	if (id_res.type == FUNCTION)
	{
		// setup args and jmp
		//AddInstruction();
	}
	else
	{
		VMRegister src{};
		src.type = INT;

		if (token->type != TOKEN_TYPE::ASSIGNMENT)
		{
			AddError("Unexpected token after int at: {}", pos);
			return;
		}

		IncrementToken();

		VMRegister dst{};
		dst.type = REGISTER;
		dst.value.num = register_pos;

		if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			// get register index 
			auto it = vars.back().find(token->str);
			if (it != vars.back().end())
			{
				dst.value = it->second.value;
				dst.type = REGISTER;
			}
			else
			{
				AddError("Undefined identifier: %s", id_res.value.str);
				return;
			}

			dst = PlusMinus();
		}
		else if (token->type == TOKEN_TYPE::NUMBER)
		{
			// get value 
			dst = PlusMinus();
		}
		else
		{
			AddError("Unexpected token after assignment at: ");
			return;
		}

		vars.back().emplace(id_res.value.str, dst);

		IncrementToken();

		if (!token)
		{
			AddError("Expected token after int at: %d", pos);
			return;
		}
		if (token->type != TOKEN_TYPE::SEMICOLON)
		{
			AddError("Unexpected token: %s. After int at: %d", token->str.c_str(), pos);
			return;
		}

		//AddInstruction(OP_CODE::OP_MOVE, { dst, src });
		//register_pos++;

		IncrementToken();
	}
}

VMRegister Expression()
{
	VMRegister res{};

	while (token)
	{
		switch (token->type)
		{
		case TOKEN_TYPE::STRING_LITERAL:
			break;
		case TOKEN_TYPE::INT:
			IntKeyword();
			break;
		case TOKEN_TYPE::IF:
			IfStatement();
			break;
		case TOKEN_TYPE::FOR:
			ForLoop();
			break;
		case TOKEN_TYPE::PARENTHESES_LEFT:
			IncrementToken();
			Expression();
			break;
		case TOKEN_TYPE::BRACKET_LEFT:
		{
			int reg_begin = register_pos;
			vars.emplace({});
			while (token->type != TOKEN_TYPE::BRACKET_RIGHT)
			{
				IncrementToken();
				Expression();
			}
			register_pos = reg_begin;
			vars.pop_back();
			break;
		}
		case TOKEN_TYPE::IDENTIFIER:
			Identifier();
			break;
		default:
			// probably encountered error but keep going so it doesnt get stuk
			IncrementToken(); 
			break;
		}

		if (errors.size() > 50)
			break;
	}


	return res;
}

void AddLibToParserCtx(const CPPLib& lib)
{
	for (const CPPFunction& f : lib.functions)
		function_sigs.emplace(f.function_sig);
}

bool Parse(const std::vector<Token>& tokens, std::vector<VM::Instruction>& op_codes_res)
{
	// recursive des$0.01
	
	parse_res.clear();
	errors.clear();
	current_tokens = tokens;

	if (current_tokens.empty())
		return false; // https://www.youtube.com/watch?v=Unzc731iCUY

	pos = 0;
	token = &current_tokens[0];

	// begin the global scope 
	vars.emplace({});
	
	AddLibToParserCtx(IO::GetIOLib());

	Expression();

	vars.pop_back();

	op_codes_res = parse_res;

	for (const std::string& err : errors)
		std::cout << err << std::endl;

	return errors.empty();
}

}
