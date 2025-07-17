#include "PreProcess.h"
#include "Lexer.h"

#include <set>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Compiler
{

namespace fs = std::filesystem;

struct PreProcessContext
{
	PreProcessContext(size_t pos, Token* token, std::vector<Token>& tokens)
		: pos(pos), token(token), current_tokens(tokens) {}

	size_t pos = 0;
	Token* token = nullptr;
	std::vector<Token>& current_tokens;

	std::set<std::string> included_paths;

	// keep track of starting token position so we can remove the tokens of the command
	size_t start_of_command = 0; 
};

static void IncrementToken(PreProcessContext& ctx)
{
	ctx.pos++;
	if (ctx.pos < ctx.current_tokens.size())
		ctx.token = &ctx.current_tokens[ctx.pos];
	else
		ctx.token = nullptr;
}

static bool ReadFile(const fs::path& file_path, std::string& out)
{
	std::ifstream file(file_path);
	if (!file)
		return false; 
		
	std::stringstream buffer;
	buffer << file.rdbuf();
	out = buffer.str();
	return true;
}

static bool PreProcessInclude(PreProcessContext& ctx)
{
	IncrementToken(ctx);
	
	if (!ctx.token)
		return false; // include requires argument 
	
	if (ctx.token->type != TOKEN_TYPE::STRING_LITERAL)
		return false; // only accept strings

	fs::path path_to_include = fs::current_path() / ctx.token->str;

	if (ctx.included_paths.contains(path_to_include))
		return true;

	if (fs::is_directory(path_to_include))
		return false; // must be a file 

	if (!fs::exists(path_to_include))
		return false; // file doesn't exist

	std::string src;
	if (!ReadFile(path_to_include, src))
		return false; // failed to read file 
	
	ctx.included_paths.emplace(fs::canonical(path_to_include).string());

	LexerInit(src);

	std::vector<Token> tokens_to_include;
	if (!LexerScan(tokens_to_include))
		return false; // Failed to tokenize included file
	
	ctx.current_tokens.erase(
		ctx.current_tokens.begin() + ctx.start_of_command, 
		ctx.current_tokens.begin() + ctx.start_of_command + ctx.pos + 1);

	ctx.current_tokens.insert(ctx.current_tokens.begin() + ctx.start_of_command, tokens_to_include.begin(), tokens_to_include.end());
	
	ctx.pos = ctx.start_of_command;

	return true;
}

const static std::unordered_map<std::string_view, std::function<bool(PreProcessContext&)>> pre_process_commands
{
	{"include", PreProcessInclude},
};

static bool GetPreProcessCommand(PreProcessContext& ctx)
{
	IncrementToken(ctx);

	if (!ctx.token)
		return false; // unexpected end 

	if (ctx.token->type != TOKEN_TYPE::IDENTIFIER)
		return false; // unexpected token

	if (!pre_process_commands.contains(ctx.token->str))
		return false; // unkown command 

	return pre_process_commands.at(ctx.token->str)(ctx);
}

bool PreProcess(std::vector<Token>& tokens)
{
	if (tokens.empty())
		return false; 

	PreProcessContext ctx(0, &tokens[0], tokens);

	for (Token& token : tokens)
	{
		ctx.token = &token;
		if (token.type == TOKEN_TYPE::HASHTAG)
		{
			ctx.start_of_command = ctx.pos;

			if (!GetPreProcessCommand(ctx))
				return false;
		}

		ctx.pos++;
	}
	
	return true;
}

}
