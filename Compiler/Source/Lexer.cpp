#include "Lexer.h"

#include <set>
#include <unordered_map>
#include <iostream>

namespace Compiler
{
	static std::string source;
	static uint32_t line = 0;
	static size_t pos = 0;

	std::unordered_map<std::string, TOKEN_TYPE> keywords
	{
		{"if", TOKEN_TYPE::IF},
		{"else", TOKEN_TYPE::ELSE},
		{"int", TOKEN_TYPE::INT},
		{"float", TOKEN_TYPE::FLOAT},
		{"for", TOKEN_TYPE::FOR},
		{"true", TOKEN_TYPE::TRUE},
		{"false", TOKEN_TYPE::FALSE},
	};

	static bool GetNextChar(char& c)
	{
		// check if we can get the next character and in bounds
		if (pos >= source.size() - 1)
			return false;

		c = source[pos + 1];

		return true;
	}

	static bool SkipComment()
	{
		// check if comment
		if (source[pos] != '/')
			return false;

		char next = 0;
		if (!GetNextChar(next))
			return false;

		if (next == '/')
		{
			// skip until next line 

			while (next != '\n')
			{
				pos++;
				if (!GetNextChar(next))
					break;
			}

			do
			{
				pos++;
				if (!GetNextChar(next))
					break;

			} while (std::isspace(next));

			pos++;

			if (next == '/')
				SkipComment();

			return true;
		}
		else if (next == '*')
		{
			// skip until */

		}

		return false;
	}

	static void CheckFinishEqualToken(Token& token)
	{
		char next = 0;
		if (!GetNextChar(next))
		{
			token.type == TOKEN_TYPE::ERROR;
			return;
		}

		// only one other posibility
		if (next == '=')
			token.type == TOKEN_TYPE::COMPARISON;
	}

	static void CheckFinishMultiplyToken(Token& token)
	{
		char next = 0;
		if (!GetNextChar(next))
		{
			token.type == TOKEN_TYPE::ERROR;
			return;
		}

		if (next == '=')
			token.type == TOKEN_TYPE::MULTIPLY_AND_ASSIGN;
		// *= ** ? !?>? PYTHONG 
	}

	static void CheckFinishDivideToken(Token& token)
	{
		char next = 0;
		if (!GetNextChar(next))
		{
			token.type == TOKEN_TYPE::ERROR;
			return;
		}

		if (next == '=')
			token.type == TOKEN_TYPE::DIVIDE_AND_ASSIGN;
	}

	// check if the word is something 
	static void CheckIdentifier(Token& token)
	{
		auto it = keywords.find(token.str);
		if (it != keywords.end())
		{
			token.type = it->second;
			return;
		}

		bool floating_point = false;

		// check if number 
		for (char c : token.str)
		{
			if (std::isspace(c))
			{
				// just in case there is still space 
				token.str.pop_back();
				continue;
			}

			if (!std::isdigit(c)) 
				return;

			if (c == '.')
				floating_point = true;
		}

		if (floating_point)
			token.type = TOKEN_TYPE::FLOAT;
		else
			token.type = TOKEN_TYPE::NUMBER;
	}

	static bool AdvanceUnfinishedToken(Token& token, char c = ' ')
	{
		if (std::isalpha(source[pos]))
		{
			for (size_t i = pos; i < source.size(); i++)
			{
				if (std::isspace(source[i]) || source[i] == c)
				{
					token.str = source.substr(pos, i - pos);
					
					if (keywords.contains(token.str))
						token.type = keywords[token.str];
					else
						token.type = TOKEN_TYPE::IDENTIFIER;

					pos = i;

					return true;
				}
				else if (source[i] == ';' || source[i] == '(' || source[i] == ')')
				{
					token.str = source.substr(pos, i - pos);
					token.type = TOKEN_TYPE::IDENTIFIER;
					pos = i - 1; // so the next token will read the semicolon 
					return true;
				}
			}
		}
		else if (std::isdigit(source[pos]))
		{
			for (size_t i = pos; i < source.size(); i++)
			{
				if (std::isdigit(source[i]))
					continue;

				if (std::isspace(source[i]))
					continue;

				if (source[i] != ';' || source[i] != ')')
				{
					token.str = source.substr(pos, i - pos);
					pos = i - 1;
					return true;
				}

				if (source[i] == c 
					|| source[i] == ';' 
					|| source[i] == ')')
				{
					token.str = source.substr(pos, i - pos);
					pos = i - 1;
					return true;
				}

				token.str = source.substr(pos, i - pos);
				pos = i - 1;
				return true;
			}
		}

		return true;
	}

	static Token GetToken(char c)
	{
		Token res;
		res.line = line;
		res.str = c;

		switch (c)
		{
		case ';':
			res.type = TOKEN_TYPE::SEMICOLON;
			break;
		case '"':
			AdvanceUnfinishedToken(res, '"');
			res.type = TOKEN_TYPE::STRING_LITERAL;
			break;
			// nuh
		case '(':
			res.type = TOKEN_TYPE::PARENTHESES_LEFT;
			break;	
		case ')':
			res.type = TOKEN_TYPE::PARENTHESES_RIGHT;
			break;
		case '=':
			res.type = TOKEN_TYPE::ASSIGNMENT;
			CheckFinishEqualToken(res);
			break;
		case '*':
			res.type = TOKEN_TYPE::MULTIPLY;
			CheckFinishMultiplyToken(res);
			break;
		case '+':
			res.type = TOKEN_TYPE::PLUS;
			//CheckFinishMultiplyToken(res);
			break;
		case '-':
			res.type = TOKEN_TYPE::MINUS;
			//CheckFinishMultiplyToken(res);
			break;
		case '/':
			res.type = TOKEN_TYPE::DIVIDE;
			CheckFinishDivideToken(res);
			break;
		default:
			res.type = TOKEN_TYPE::IDENTIFIER;
			if (AdvanceUnfinishedToken(res, ' '))
				CheckIdentifier(res);
			break;
		}

		return res;
	}

	void LexerInit(std::string_view src)
	{
		source = src;
		line = 0;
		pos = 0;
	}

	bool LexerScan(std::vector<Token>& tokens)
	{
		while (pos < source.size())
		{
			char c = source[pos];

			// skip whitespace
			while (std::isspace(c)) 
			{
				pos++;
				if (pos >= source.size())
					return true;

				c = source[pos];
			}

			if (SkipComment())
			{
				// update to character after comment 
				c = source[pos];
			}

			Token token = GetToken(c);
			tokens.emplace_back(token);
			std::cout << token.str << ' ' << (int)token.type << std::endl;

			if (token.type == TOKEN_TYPE::ERROR)
				return false;

			pos++;
		}

		return true;
	}

}