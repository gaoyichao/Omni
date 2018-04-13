#include <token.h>
#include <mm.h>

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

