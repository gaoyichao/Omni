#ifndef LEXER_H
#define LEXER_H




typedef enum eToken
{
    TK_BEGIN,
#define TOKEN(k, s) k,
#include <token.h>
#undef  TOKEN
    TK_BADCHAR
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




#endif // LEXER_H
