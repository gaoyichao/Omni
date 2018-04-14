#ifndef TOKEN_H
#define TOKEN_H

#include <types.h>

typedef enum eToken
{
    TK_Begin = 0,
    TK_Id,
    TK_Keyword,
    TK_Variable,
    TK_Function,
    TK_IntegerConstant,
    TK_FloatingConstant,
    TK_CharConstant,
    TK_StringLiteral,
    TK_BadChar,
    TK_Add, // +
    TK_Sub, // -
    TK_Mul, // *
    TK_Div, // /
    TK_End
} eToken;

#define VECTOR_NUMBER
#define T eToken
#define TEMPLATE_TYPE(a) a ##_eToken
#define TEMPLATE_FUNCTION(a,b) a ##_eToken_## b
#include <vector/vector_t_h.h>
#include <vector/vector_t_num_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER




struct Token {
    eToken token;
    uint8 *str;
    int line;
    int col;
};

struct Token *CreateToken(eToken type, const uint8 *buf, int len);
void DestroyToken(struct Token *token);


#endif
