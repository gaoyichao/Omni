#include <token.h>
#include <mm.h>

#define VECTOR_NUMBER
#define T eToken
#define TEMPLATE_TYPE(a) a ##_eToken
#define TEMPLATE_FUNCTION(a,b) a ##_eToken_## b
#include <vector/vector_t_c.h>
#include <vector/vector_t_num_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

#define T Token
#define TEMPLATE_TYPE(a) a ##_Token
#define TEMPLATE_FUNCTION(a,b) a ##_Token_## b
#include <vector/vector_t_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

struct TokenList* CreateTokenList() {
    struct TokenList *re = Calloc(1, struct TokenList);
    vector_Token_init(&(re->tokens), 0);
    vector_uint8_init(&(re->strings), 0);
    return re;
}

void TokenList_Clear(struct TokenList *tklist) {
    vector_Token_clear(&(tklist->tokens));
    vector_uint8_clear(&(tklist->strings));
}

void DestroyTokenList(struct TokenList *tklist) {
    vector_Token_destroy(&(tklist->tokens));
    vector_uint8_destroy(&(tklist->strings));
    Free(tklist);
}
/*
 * TokenList_NewItem - 获取新对象
 */
struct Token *TokenList_NewItem(struct TokenList *tklist, eToken type, const uint8 *buf, int len) {
    uint8 *before = tklist->strings.stor_begin;
    int nItems = vector_Token_size(&(tklist->tokens));

    struct Token *re = vector_Token_new_item(&(tklist->tokens));
    re->token = type;
    re->line = 0;
    re->col = 0;
    re->str = vector_uint8_new_items(&(tklist->strings), len+1);
    // 申请更多字符串时,有可能重新向系统请求内存,此时需要调整token中str指针
    uint8 *after = tklist->strings.stor_begin;

    if (before != after) {
        int diff = after - before;
        for (int i = 0; i < nItems; i++) {
            VECTOR(tklist->tokens)[i].str += diff;
        }
    }

    memcpy(re->str, buf, len);
    re->str[len] = '\0';
    return re;
}
/*
 * TokenList_GetItem - 获取原对象
 */
struct Token *TokenList_GetItem(struct TokenList *tklist, int n) {
    return &VECTOR(tklist->tokens)[n];
}




