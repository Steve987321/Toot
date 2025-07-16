#include "gtest/gtest.h"

#include "Lexer.h"

TEST(Lexer, Whitespace)
{
    using namespace Compiler;

    std::vector<Token> output_tokens;
    LexerInit(" int \n i \n =  \n 0 \n ; \n");
    ASSERT_TRUE(LexerScan(output_tokens));
    EXPECT_EQ(output_tokens.size(), 5);

    output_tokens.clear();
    
    LexerInit("i\nn\nt");
    ASSERT_TRUE(LexerScan(output_tokens));
    EXPECT_EQ(output_tokens.size(), 3);
    for (const Token& token : output_tokens)
        ASSERT_EQ(token.type, TOKEN_TYPE::IDENTIFIER);
}

TEST(Lexer, KeywordTokens)
{
    using namespace Compiler;

    std::vector<Token> output_tokens;
    LexerInit("int");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].str, "int");
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::INT);

    output_tokens.clear();

    LexerInit("float");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].str, "float");
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::FLOAT);
}

TEST(Lexer, IgnoreComments)
{
    using namespace Compiler;

    std::vector<Token> output_tokens;
    LexerInit("// int i = 0;");
    ASSERT_TRUE(LexerScan(output_tokens));
    EXPECT_TRUE(output_tokens.empty());

    output_tokens.clear();

    LexerInit("int i = 0; // int j = 0;");
    ASSERT_TRUE(LexerScan(output_tokens));
    EXPECT_EQ(output_tokens.size(), 5);

    output_tokens.clear();

    LexerInit("int i = 0; /// int j = 0; // /// //");
    ASSERT_TRUE(LexerScan(output_tokens));
    EXPECT_EQ(output_tokens.size(), 5);
}

TEST(Lexer, IdentifyKeywords)
{
    using namespace Compiler;

    std::vector<Token> output_tokens;
    LexerInit("int");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::INT);

    output_tokens.clear();

    LexerInit("Int");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::IDENTIFIER);

    output_tokens.clear();

    LexerInit("float");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::FLOAT);

    output_tokens.clear();

    LexerInit("if");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::IF);

    output_tokens.clear();

    LexerInit("once");
    ASSERT_TRUE(LexerScan(output_tokens));
    ASSERT_EQ(output_tokens.size(), 1);
    EXPECT_EQ(output_tokens[0].type, TOKEN_TYPE::ONCE);
    // some more
}

