#include <parser.h>
#include <stdlib.h>

#define VECTOR_NUMBER
#define T ExpressionPtr
#define TEMPLATE_TYPE(a) a ##_ExpressionPtr
#define TEMPLATE_FUNCTION(a,b) a ##_ExpressionPtr_## b
#include <vector/vector_t_c.h>
#include <vector/vector_t_num_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

struct Expression *CreatePrimaryExp_Constant(struct Token *tk) {
    struct Expression *re = (struct Expression*)malloc(sizeof(struct Expression));
    re->type = ET_PrimaryExp_Constant;
    vector_ExpressionPtr_init(&(re->subExp), 0);
    re->content.constant = atof((const char*)tk->str);

    return re;
}

void DestroyExpression(struct Expression *exp) {
    vector_ExpressionPtr_destroy(&(exp->subExp));

    free(exp);
}

double CalcExpression(struct Expression *exp) {
    switch (exp->type) {
    case ET_PrimaryExp_Constant:
        return exp->content.constant;
    default:
        return 0;
    }
}


struct Parser *CreateParser() {
    struct Parser *re = (struct Parser*)malloc(sizeof(struct Parser));

    return re;
}

void ParserSetLexer(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
}

void DestroyParser(struct Parser *parser) {
    free(parser);
}

