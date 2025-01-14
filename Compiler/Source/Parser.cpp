#include "Parser.h"

#include <cassert>
#include <iostream>
#include <set>
#include <format>
#include <stack>

namespace Compiler
{

using namespace TVM;

struct FunctionArgs
{

};
	
static size_t pos = 0;
static Token* token = nullptr;
static std::vector<Token> current_tokens;
static std::vector<std::string> errors;

static size_t register_pos = 0;
static std::set<std::string> function_sigs;
static std::stack<std::unordered_map<std::string_view, Register>> vars;

static std::vector<VM::Instruction> parse_res;

extern Register Expression();

template<typename ...TArgs> 
static void AddError(std::string_view format, TArgs... args)
{
	char buf[128]{};
	snprintf(buf, sizeof(buf), format.data(), args...);
	std::cout << buf << std::endl;
	errors.emplace_back(buf);
}

static Register CreateRegisterDst(int reg)
{
	Register dst{};
	dst.value.num = reg;
	dst.type = REGISTER;
	return dst;
}

static Register* GetVarRegister(const std::string& id)
{
	auto pos = vars.top().find(id);
	if (pos != vars.top().end())
		return &pos->second;

	return nullptr;
}

// temp
static void PrintInstruction(TVM::OP_CODE op_code, std::vector<TVM::Register> args = {})
{
	std::string s = op_code_names[op_code] + ' ';
	for (const Register& arg : args)
	{
		switch (arg.type)
		{
		case TVM::RegisterType::FLOAT:
			s += std::to_string(arg.value.flt);
			break;
		case TVM::RegisterType::INT:
			s += std::to_string(arg.value.num);
			break;
		case TVM::RegisterType::STRING:
			s += arg.value.str;
			break;
		case TVM::RegisterType::REGISTER:
			s += 'r' + std::to_string(arg.value.num);
			break;
		}
		s += ' ';
	}

	std::cout << s << std::endl;
}

static void AddInstruction(TVM::OP_CODE op_code, std::vector<TVM::Register> args = {})
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

static std::string RegToType(const Register& reg)
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
static std::string RegsToTypes(const std::vector<TVM::Register>& args)
{
	std::string res;

	for (const Register& reg : args)
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

extern Register PlusMinus();

static Register Unary()
{
	Register res {};
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
				Register dst = CreateRegisterDst(register_pos);
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
		Register* reg = GetVarRegister(token->str);
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

		Register src{};
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

static Register Factor()
{
	Register a = Unary();
	if (a.value.num == -1)
	{
		AddError("Unexpected token after: %s", token->str.c_str());
	}

	Register dst = a;

	Token* next = PeekNextToken();
	if (next && (next->type == TOKEN_TYPE::MULTIPLY || next->type == TOKEN_TYPE::DIVIDE))
	{
		IncrementToken();

		while (next && (next->type == TOKEN_TYPE::MULTIPLY || next->type == TOKEN_TYPE::DIVIDE))
		{
			IncrementToken();
			Register b = Unary();
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

static Register PlusMinus()
{
	Register a = Factor();
	if (a.value.num == -1)
	{
		AddError("Unexpected token after: {}", token->str);
	}

	Register dst = a;

	Token* next = PeekNextToken();
	if (next && (next->type == TOKEN_TYPE::PLUS || next->type == TOKEN_TYPE::MINUS))
	{
		IncrementToken();

		while (next && (next->type == TOKEN_TYPE::PLUS || next->type == TOKEN_TYPE::MINUS))
		{
			IncrementToken();
			int rp_prev = register_pos;
			Register b = Factor();
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

static Register IfStatement()
{
	return {};
}

static Register ForLoop()
{
	return {  };
}

static Register Identifier()
{
	Register res{};
	res.type = STRING;
	res.value.str = token->str.c_str();

	IncrementToken();

	if (!token)
		return res;

	// check if function call or definition
	if (token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		res.type = FUNCTION;
		std::vector<Register> args_as_registers{};
		std::string arg_list;

		for (size_t i = pos; i < current_tokens.size(); i++)
		{
			Token& t = current_tokens[i];

			if (t.type == TOKEN_TYPE::IDENTIFIER)
			{
				Register* reg = GetVarRegister(t.str);

				// #TODO VALIDATE FOR NUM VALUEs
				if (reg)
				{
					pos = i;
					token = &current_tokens[i];
					// numerical
					Register arg = PlusMinus();

					// check if -1 and in that case it probably ended the expression
					if (arg.value.num != -1)
					{
						assert(arg.type == REGISTER);
						args_as_registers.emplace_back(arg);
						i = pos;
					}
				}
			}

			if (t.type == TOKEN_TYPE::NUMBER || t.type == TOKEN_TYPE::FLOAT)
			{
				pos = i;
				token = &current_tokens[i];
				// numerical
				Register arg = PlusMinus();

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

				if (next->type == TOKEN_TYPE::BRACKET_LEFT)
				{
					// definition
					auto stored_sig = function_sigs.emplace(sig);
					Register func{};
					func.type = STRING;
					func.value.str = stored_sig.first->c_str();

					AddInstruction(OP_LABEL, {func});

					while (token->type != TOKEN_TYPE::BRACKET_RIGHT)
						Expression();

					AddInstruction(OP_RETURN);
				}
				else if (next->type == TOKEN_TYPE::SEMICOLON)
				{
					// call
					std::string func_name = sig.substr(0, sig.find(' '));
					if (function_sigs.contains(func_name + " ..."))
						sig = func_name + " ...";
					else if (!function_sigs.contains(sig))
					{
						AddError("No function found with signature %s", sig.c_str());
						return res;
					}

					// #TODo: temp leak for now 
					Register function_arg{};
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

	Register id_res = Identifier();
	
	if (id_res.type == FUNCTION)
	{
		// setup args and jmp
		//AddInstruction();
	}
	else
	{
		TVM::Register src{};
		src.type = INT;

		if (token->type != TOKEN_TYPE::ASSIGNMENT)
		{
			AddError("Unexpected token after int at: {}", pos);
			return;
		}

		IncrementToken();

		TVM::Register dst{};
		dst.type = REGISTER;
		dst.value.num = register_pos;

		if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			// get register index 
			auto it = vars.top().find(token->str);
			if (it != vars.top().end())
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

		vars.top().emplace(id_res.value.str, dst);

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

		//AddInstruction(TVM::OP_CODE::OP_MOVE, { dst, src });
		//register_pos++;

		IncrementToken();
	}
}

Register Expression()
{
	Register res{};

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
			vars.emplace();
			while (token->type != TOKEN_TYPE::BRACKET_RIGHT)
			{
				IncrementToken();
				Expression();
			}
			register_pos = reg_begin;
			vars.pop();
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

bool Parse(const std::vector<Token>& tokens, std::vector<VM::Instruction>& op_codes_res)
{
	// recursive des$0.01
	
	parse_res.clear();
	errors.clear();
	current_tokens = tokens;

	if (current_tokens.empty())
		return false; // broterman

	pos = 0;
	token = &current_tokens[0];

	// begin the global scope 
	vars.emplace();
	
	for (CPPFunction& function : IO::GetIOLib().functions)
	{
		function_sigs.emplace(std::string(function.function_name) + ' ' + function.accepted_args);
	}

	Expression();

	vars.pop();

	op_codes_res = parse_res;

	for (const std::string& err : errors)
		std::cout << err << std::endl;

	return errors.empty();
}

}
