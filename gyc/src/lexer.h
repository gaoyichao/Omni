#ifndef LEXER_H
#define LEXER_H

#include <types.h>

typedef enum eToken
{
    TK_Begin = 0,
    TK_Id,
    TK_Keyword,
    TK_IntegerConstant,
    TK_FloatingConstant,
    TK_CharConstant,
    TK_StringLiteral,
    TK_BadChar,
    TK_End
} eToken;

struct Token {
    eToken token;
    uint8 *str;
    int line;
    int col;
};

#include <dictionary.h>
#include <inputs.h>

struct Lexer;
typedef eToken (*ScanFunc)(struct Lexer *lexer);

struct Lexer {
    struct Inputs *inputs;

    struct Dictionary *keyWordsDic;
    ScanFunc scanners[256];
};


struct Lexer *CreateLexer();
void LexerLoadKeywords(struct Lexer *lexer, char *filename);
void LexerSetInputs(struct Lexer *lexer, struct Inputs *inputs);
void DestroyLexer(struct Lexer *lexer);

struct Token *GetNextToken(struct Lexer *lexer);
void DestroyToken(struct Token *token);



#endif // LEXER_H
