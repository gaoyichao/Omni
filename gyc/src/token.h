#ifndef TOKEN_H
#define TOKEN_H

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

struct Token *CreateToken(eToken type, const uint8 *buf, int len);
void DestroyToken(struct Token *token);


#endif
