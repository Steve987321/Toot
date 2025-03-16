#include "Parser.h"

#include <cassert>
#include <iostream>
#include <set>
#include <format>
#include <stack>

namespace Compiler
{
	
static size_t pos = 0;
static Token* token = nullptr;
static std::vector<Token> current_tokens;
static std::vector<std::string> errors;

static size_t register_pos = 0;
static std::set<std::string> function_sigs;
static std::vector<std::unordered_map<std::string_view, VMRegister>> vars;

static std::vector<VM::Instruction> parse_res;

extern VMRegister Expression();
static VMRegister PlusMinus();

template<typename ...TArgs> 
static void AddError(std::string_view format, TArgs... args)
{
	char buf[128]{};
	snprintf(buf, sizeof(buf), format.data(), args...);
	// std::cout << buf << std::endl;
	errors.emplace_back(buf);
}

static bool VarExists(std::string_view id)
{
    for (size_t i = vars.size(); i-- > 0; )
    {
        if (vars[i].contains(id))
            return true;
    }
    return false;
}

static VMRegister CreateRegisterDst(size_t reg)
{
	VMRegister dst{};
	dst.value.num = reg;
	dst.type = VMRegisterType::REGISTER;
	return dst;
}

static VMRegister* GetVarRegister(const std::string& id)
{
	for (auto& v : vars)
	{
		auto pos = v.find(id);
		if (pos != v.end())
			return &pos->second;
	}

	return nullptr;
}

static VMRegister CreateUniqueLabelRegister()
{
	static int counter = 0;
	VMRegister reg;
	reg.type = VMRegisterType::INT;
	reg.value.num = counter;
	counter++;
    return reg;
}

// temp
static void PrintInstruction(OP_CODE op_code, std::vector<VMRegister> args = {}, VMRegister reserved = {0, VMRegisterType::INVALID})
{
	std::string s = op_code_names[op_code] + ' ';
	args.emplace_back(reserved);
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
		default:
			break;
		}
		s += ' ';
	}

	std::cout << s << std::endl;
}

static void AddInstruction(OP_CODE op_code, std::vector<VMRegister> args = {}, VMRegister reserved = {0, VMRegisterType::INVALID})
{
	// temp
	PrintInstruction(op_code, args, reserved);

	parse_res.emplace_back(op_code, args, reserved);
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
	case VMRegisterType::INVALID:
		return "invalid";
	case VMRegisterType::REGISTER:
		return "register";
	case VMRegisterType::INT:
		return "int";
	case VMRegisterType::STRING:
		return "string";
    case VMRegisterType::FLOAT:
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

static Token* PeekPreviousToken(int steps = 1)
{
	Token* res = nullptr;
	if (pos != 0)
		res = &current_tokens[pos - steps];

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

static void BeginInnerScope()
{
    size_t reg_begin = register_pos;
    vars.push_back({});
    while (token->type != TOKEN_TYPE::BRACKET_RIGHT)
    {
        IncrementToken();
        Expression();
    }
    register_pos = reg_begin;
    vars.pop_back();
}

static bool GetFunctionArgs(const std::string& func_name, std::vector<VMRegister>& args_as_registers, std::string& sig)
{
    std::string arg_list;
    sig.clear();
    
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
					assert(arg.type == VMRegisterType::REGISTER);
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
				assert(arg.type == VMRegisterType::REGISTER);
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

			sig = func_name;
			sig += ' ';
			sig += RegsToTypes(args_as_registers);

			Token* next = PeekNextToken();
			if (!next)
			{
				AddError("Expected token at:");
				return false;
			}

			// check if definition
			if (next->type == TOKEN_TYPE::BRACKET_LEFT)
			{
				// if (store_return)
				// {
				// 	AddError("Can't assign to function definition");
				// 	return;
				// }

				auto [stored_sig, _] = function_sigs.emplace(sig);
				VMRegister func{};
				func.type = VMRegisterType::STRING;
				func.value.str = stored_sig->c_str();

				AddInstruction(OP_DEFINE_LABEL, {func});

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
						return false;
					}
				}

				// if (store_return)
				// {
					
				// }
			
				break;
			}
		}
	}

	return true;
}

static void CallFunctionEx(std::string_view sig, std::vector<VMRegister>& args_as_registers, const VMRegister& dst)
{
	// #tood: temp leak for now 
	VMRegister function_arg{};
	function_arg.type = VMRegisterType::STRING;
	void* p = malloc(sig.size());
	assert(p);
	memcpy(p, sig.data(), sig.size() + 1);
	function_arg.value.str = (char*)p;
	args_as_registers.insert(args_as_registers.begin(), function_arg);
	
	if (dst.value.num > 0)
		AddInstruction(OP_CALL_MOVE, args_as_registers, dst);
	else
		AddInstruction(OP_CALL, args_as_registers);
}

static void CallFunction(std::string_view sig, std::vector<VMRegister>& args_as_registers)
{
	VMRegister dst;
	dst.value.num = -1;
	CallFunctionEx(sig, args_as_registers, dst);
}

static VMRegister Unary()
{
	VMRegister res {};
	res.type = VMRegisterType::INT;
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
		res.type = VMRegisterType::REGISTER;

		// get var register
		VMRegister* reg = GetVarRegister(token->str);
		if (!reg)
		{
			// #todo: check function signature
            Token* next = PeekNextToken();
			if (next && next->type == TOKEN_TYPE::PARENTHESES_LEFT)
			{
                std::string func_name = token->str;
                std::string sig;
				std::vector<VMRegister> args_as_registers;
                
                IncrementToken();
				if (GetFunctionArgs(func_name, args_as_registers, sig))
				{
					res = CreateRegisterDst(register_pos);
					register_pos++;
                    CallFunctionEx(sig, args_as_registers, res);
					return res;
				}
			}

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
		res.type = VMRegisterType::REGISTER;
		int val = std::stoi(token->str);

		if (is_unary_min)
			val = -val;

		VMRegister src{};
		src.type = VMRegisterType::INT;
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
		AddError("Unexpected token after: {}", token->str.c_str());
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

static VMRegister ForLoop()
{
	return {};
}

static VMRegister Identifier()
{
	VMRegister res{};
	res.type = VMRegisterType::STRING;
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
					AddError("Variable with identifier {} already exists", token->str.c_str());
					return res;
				}
				else
				{
					// get the variable and chec
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
//                    else if (IsTokenOperator(*token))
//                    {
//                        
//                    }
//                    else if (IsTokenComparison(*token))
//                    {
//                        
//                    }
				}
			}
			
			return res;
		}
	}

    // bool store_return = false;
    // VMRegister store_return_reg;
	// before checking for function check if it discards the return type
	// Token* prev = PeekPreviousToken();
	// if (prev && prev->type == TOKEN_TYPE::ASSIGNMENT)
	// {
	// 	Token* next = PeekNextToken();
	// 	if (next && next->type == TOKEN_TYPE::PARENTHESES_LEFT)
	// 		store_return = true; 
	// 		// #todo: should check matching return type

	// 	// where to store the return value
	// 	Token* prev_var = PeekPreviousToken(2);
	// }

	IncrementToken();

	if (!token)
		return res;

	// check if function call or definition
	if (token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		res.type = VMRegisterType::FUNCTION;
		std::string sig;
		std::vector<VMRegister> args_as_registers;
		if (GetFunctionArgs(res.value.str, args_as_registers, sig))
		{
			CallFunction(sig, args_as_registers);
		}
	}

	return res;
}

// for the if statement ()
// static void GetComparisonArgs(VMRegister& a, VMRegister& b)
// {

// }

static VMRegister IfStatement()
{
	IncrementToken();

	if (token && token->type == TOKEN_TYPE::PARENTHESES_LEFT)
	{
		VMRegister a;
		VMRegister b;

        IncrementToken();
		// get first argument
		if (token->type == TOKEN_TYPE::NUMBER)
		{
			a = PlusMinus();
		}
		else if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			a = Identifier();
		}
		else
		{
			AddError("after '(' Unexpected token: {}", token->str.c_str());
			return {};
		}

		// get comparison
		if (!token)
		{
			AddError("unexpected end");
			return {};
		}

		Token& comp_type = *token;

		// get second argument
		IncrementToken();

		if (!token)
		{
			AddError("unexpected end");
			return {};
		}
		if (token->type == TOKEN_TYPE::NUMBER)
		{
			b = PlusMinus();
		}
		else if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			b = Identifier();
		}
		else
		{
			AddError("after '{}' unexpected token: {}", comp_type.str.c_str(), token->str.c_str());
			return {};
		}
		
		// check branches 
		IncrementToken();

		if (!token)
		{
			AddError("Unexected end");
			return {};
		}
		if (token->type != TOKEN_TYPE::PARENTHESES_RIGHT)
		{
			AddError("Unexpected token {}, expected ')'", token->str.c_str());
			return {};
		}
	
		// check first branch 
		IncrementToken();
		if (!token)
		{
			AddError("Unexected end");
			return {};
		}
		if (token->type != TOKEN_TYPE::BRACKET_LEFT)
		{
			AddError("Unexpected token {}, expected ')'", token->str.c_str());
			return {};
		}

		bool end = false;
		bool has_else = false; 
		// check for end and check for else or else if's 
		for (size_t i = pos; i < current_tokens.size(); i++)
		{
			const Token* t = &current_tokens[i];
			if (t->type == TOKEN_TYPE::BRACKET_RIGHT)
			{
				end = true;
				
				i++;
				if (i < current_tokens.size())
				{
					t = &current_tokens[i];
					if (t->type == TOKEN_TYPE::ELSE)
						has_else = true;
				}
				
				break;
			}
		}

		if (!end)
		{
			AddError("Expected closing bracket '}'");
			return {};
		}

		// branch destinations 
//		VMRegister if_branch = CreateUniqueLabelRegister();
		VMRegister if_end = CreateUniqueLabelRegister();
		VMRegister if_else = {};
		if (has_else)
			if_else = CreateUniqueLabelRegister();
		
		// check comparison type 
		if (comp_type.type == TOKEN_TYPE::COMPARISON)
		{
			// == 
			// go to the end if it evaluates to false 
			AddInstruction(OP_CODE::OP_JUMP_IF_NOT_EQUAL, {if_end, a, b});
		}
		
		// define branches 
//		AddInstruction(OP_DEFINE_LABEL, {if_branch});
		
        BeginInnerScope();
        
        if (token && token->type == TOKEN_TYPE::BRACKET_RIGHT)
        {
            IncrementToken();
        }
        else
        {
            AddError("Expected closing bracket '}'");
            return {};
        }
        
		// #todo can use has_else instead of checking again
        if (token && token->type == TOKEN_TYPE::ELSE)
        {
            AddInstruction(OP_DEFINE_LABEL, {if_else});
            
            IncrementToken();
            BeginInnerScope();
            IncrementToken();
        }

		AddInstruction(OP_DEFINE_LABEL, {if_end});
	}
	return {};
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
	
	if (id_res.type == VMRegisterType::FUNCTION)
	{
		// setup args and jmp
		//AddInstruction();
	}
	else
	{
		VMRegister src{};
		src.type = VMRegisterType::INT;

		if (token->type != TOKEN_TYPE::ASSIGNMENT)
		{
			AddError("Unexpected token after int at: {}", pos);
			return;
		}

		IncrementToken();

		VMRegister dst{};
		dst.type = VMRegisterType::REGISTER;
		dst.value.num = (int32_t)register_pos;

		if (token->type == TOKEN_TYPE::IDENTIFIER)
		{
			// get register index
			VMRegister* reg = GetVarRegister(token->str);
			if (reg)
			{
				dst.value = reg->value;
				dst.type = VMRegisterType::REGISTER;
			}
			else
			{
                Token* next = PeekNextToken();
                if (!next || next->type != TOKEN_TYPE::PARENTHESES_LEFT)
                {
                    AddError("Undefined identifier: %s", id_res.value.str);
                    return;
                }
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
            BeginInnerScope();
            IncrementToken();
			break;
		}
        case TOKEN_TYPE::BRACKET_RIGHT:
        {
            return {};
            break;
        }
		case TOKEN_TYPE::IDENTIFIER:
        {
            VMRegister id = Identifier();
            if (id.type == VMRegisterType::STRING && !VarExists(id.value.str))
                AddError("Undefined identifier %s", id.value.str);
            break;
        }
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
    
    for (const auto& [name, val] : lib.vars)
    {
        VMRegister reg = CreateRegisterDst(register_pos);
        register_pos++;
        
        AddInstruction(OP_MOVE, {reg, val});
        vars.back().emplace(name, reg);
    }
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
	vars.push_back({});
	
	AddLibToParserCtx(IO::GetIOLib());

	Expression();

	vars.pop_back();

	op_codes_res = parse_res;

	for (const std::string& err : errors)
		std::cout << err << std::endl;

	return errors.empty();
}

}
