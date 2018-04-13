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

struct Token *CreateToken(eToken type, const uint8 *buf, int len) {
    struct Token *re = Calloc(1, struct Token);
    re->line = 0;
    re->col = 0;
    re->token = type;
    re->str = Calloc(len+1, uint8);
    memcpy(re->str, buf, len);
    re->str[len] = '\0';
    return re;
}

void DestroyToken(struct Token *token) {
    Free(token->str);
    Free(token);
}

