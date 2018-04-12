#ifndef LEXER_H
#define LEXER_H

typedef enum eToken
{
    TK_Begin = 0,
    TK_Id,
    TK_IntegerConstant,
    TK_FloatingConstant,
    TK_CharConstant,
    TK_StringLiteral,
    TK_BadChar
} eToken;

struct Token {
    eToken token;
    uint8 *str;
    int line;
    int col;
};

struct Lexer;
typedef eToken (*ScanFunc)(struct Lexer *lexer);

struct Lexer {
    struct Inputs *inputs;

    ScanFunc scanners[256];
};


struct Lexer *CreateLexer(struct Inputs *inputs);

struct Token *GetNextToken(struct Lexer *lexer);
void DestroyToken(struct Token *token);



#endif // LEXER_H
