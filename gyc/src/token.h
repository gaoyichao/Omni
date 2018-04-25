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
    TK_Comment,
    TK_BadChar,
    TK_Add,             // +
    TK_Sub,             // -
    TK_Mul,             // *
    TK_Div,             // /
    TK_LParenthesis,    // (
    TK_RParenthesis,    // )
    TK_Assign,          // =
    TK_Hash,            // #
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

typedef struct Token {
    eToken token;
    uint8 *str;
    int line;
    int col;
} Token;

#define T Token
#define TEMPLATE_TYPE(a) a ##_Token
#define TEMPLATE_FUNCTION(a,b) a ##_Token_## b
#include <vector/vector_t_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#include <vector_uint8.h>

struct TokenList {
    vector_Token tokens;            // Token列表,在词法分析过程中不断扩充
    vector_uint8 strings;           // Token对应的字符串,在词法分析过程中不断扩充
};

struct TokenList* CreateTokenList();
void TokenList_Clear(struct TokenList *tklist);
void DestroyTokenList(struct TokenList *tklist);

struct Token *TokenList_NewItem(struct TokenList *tklist, eToken type, const uint8 *buf, int len);
struct Token *TokenList_GetItem(struct TokenList *tklist, int n);

#endif
