#include "Parser.h"

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
	errors.emplace_back(buf);
}

static Register CreateRegisterDst(int reg)
{
	Register dst{};
	dst.value.num = reg;
	dst.type = REGISTER;
	return dst;
}

// returns .value.num=-1 if id not found
static Register* GetVarRegister(const std::string& id)
{
	auto pos = vars.top().find(id);
	if (pos != vars.top().end())
		return &pos->second;

	return nullptr;
}

static void AddInstruction(TVM::OP_CODE op_code, std::vector<TVM::Register> args = {})
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
		Register reg_res = PlusMinus();
		if (token->type == TOKEN_TYPE::PARENTHESES_RIGHT)
		{
			if (is_unary_min || is_not)
			{
				Register dst = CreateRegisterDst(register_pos);
				res.value.num = register_pos;
				register_pos++;

				if (is_unary_min)
					AddInstruction(OP_NEGATE, { reg_res });
				else if (is_not)
					AddInstruction(OP_NOT, { reg_res });

				AddInstruction(OP_MOVE, { dst, reg_res });
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
		AddError("Unexpected token after: ");
	}

	Register dst = a;

	Token* next = PeekNextToken();
	if (next && next->type == TOKEN_TYPE::MULTIPLY || token->type == TOKEN_TYPE::DIVIDE)
	{
		IncrementToken();

		while (token->type == TOKEN_TYPE::MULTIPLY || token->type == TOKEN_TYPE::DIVIDE)
		{
			IncrementToken();
			Register b = Unary();
			dst = CreateRegisterDst(register_pos);
			register_pos++;
			AddInstruction(OP_MULTIPLY, { dst, a, b });
		}
	}

	return dst;
}

static Register PlusMinus()
{
	Register a = Factor();
	if (a.value.num == -1)
	{
		AddError("Unexpected token after: ");
	}

	Register dst{};

	Token* next = PeekNextToken();
	if (next && next->type == TOKEN_TYPE::PLUS || token->type == TOKEN_TYPE::MINUS)
	{
		IncrementToken();

		while (token->type == TOKEN_TYPE::PLUS || token->type == TOKEN_TYPE::MINUS)
		{
			IncrementToken();
			Register b = Factor();
			dst = CreateRegisterDst(register_pos);
			register_pos++;
			AddInstruction(OP_ADD, { dst, a, b });
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

	// check if function call or definition
	if (token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		res.type = FUNCTION;
		std::string arg_list = token->str;

		for (int i = pos; i < current_tokens.size(); i++)
		{
			Token& t = current_tokens[i];
			arg_list += t.str;

			// end of args 
			if (t.type == TOKEN_TYPE::PARENTHESES_RIGHT)
			{
				std::string sig = res.value.str + arg_list;

				// globgogabgalab
				//TOKEN_TYPE& next = current_tokens[i + 1].type;
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

					if (!function_sigs.contains(sig))
						AddError("No function found with");

					Register function_arg{};
					function_arg.type = STRING;
					function_arg.value.str = sig.c_str();

					//AddInstruction(OP_MOVE);
					//AddInstruction(OP_CALL, { function_arg,  });
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
		// store location of var
		TVM::Register dst_reg_loc{};
		dst_reg_loc.type = REGISTER;
		dst_reg_loc.value.num = register_pos;

		vars.top().emplace(id_res.value.str, dst_reg_loc);

		TVM::Register src{};
		src.type = INT;

		if (token->type != TOKEN_TYPE::ASSIGNMENT)
		{
			AddError("Unexpected token after int at: {}", pos);
			return;
		}

		IncrementToken();

		TVM::Register dst{};
		dst.type = INT;
		dst.value.num = register_pos;

		if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			// get register index 
			auto it = vars.top().find(id_res.value.str);
			if (it != vars.top().end())
			{
				dst.value = it->second.value;
				dst.type = REGISTER;
			}
			else
			{
				AddError("Undefined identifier: {}", token->str);
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
	// recursive de$0.01

	parse_res.clear();
	errors.clear();
	current_tokens = tokens;

	if (current_tokens.empty())
		return false; // broterman

	pos = 0;
	token = &current_tokens[0];

	// begin the global scope 
	vars.emplace();

	Expression();

	vars.pop();

	op_codes_res = parse_res;

	for (const std::string& err : errors)
		std::cout << err << std::endl;

	return errors.empty();
}

}
