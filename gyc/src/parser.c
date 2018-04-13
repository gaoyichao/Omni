#include <parser.h>
#include <stdlib.h>
#include <mm.h>

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

struct Expression *CreateExpression(eExpType type) {
    struct Expression *re = Calloc(1, struct Expression);
    re->type = type;
    vector_ExpressionPtr_init(&(re->subExp), 0);
    vector_eToken_init(&(re->operators), 0);

    return re;
}

struct Expression *CreatePrimaryExp_Constant(struct Token *tk) {
    struct Expression *re = CreateExpression(ET_PrimaryExp_Constant);
    re->content.constant = atof((const char*)tk->str);
    return re;
}

struct Expression *CreatePrimaryExp_Variable(struct Symbol *symbol) {
    struct Expression *re = CreateExpression(ET_PrimaryExp_Variable);
    re->content.variable = symbol;
    return re;
}

struct Expression *CreateAdditiveExp(struct Expression *l, eToken op, struct Expression *r) {
    struct Expression *re = 0;
    assert(IsAdditiveExpression(l) && IsAdditiveExpression(r));


    if (ET_AdditiveExp == l->type) {
        re = l;
    } else {
        re = CreateExpression(ET_AdditiveExp);
        vector_ExpressionPtr_push_back(&(re->subExp), l);
    }

    vector_eToken_push_back(&(re->operators), op);

    if (ET_AdditiveExp == r->type) {
        vector_ExpressionPtr_append(&(r->subExp), &(re->subExp));
        vector_eToken_append(&(r->operators), &(re->operators));
        DestroyExpression(r);
    } else {
        vector_ExpressionPtr_push_back(&(re->subExp), r);
    }

    return re;
}


void DestroyExpression(struct Expression *exp) {
    vector_ExpressionPtr_destroy(&(exp->subExp));
    vector_eToken_destroy(&(exp->operators));

    free(exp);
}

double CalcPrimaryExp_Variable(struct Expression *exp) {
    struct Symbol *symbol = exp->content.variable;
    if (0 == symbol->vptr)
        return 0.0;
    return 1.0;
}

double CalcAdditiveExp(struct Expression *exp) {
    double left = CalcExpression(VECTOR(exp->subExp)[0]);
    int n = vector_eToken_size(&(exp->operators));

    for (int i = 0; i < n; i++) {
        eToken op = VECTOR(exp->operators)[i];
        assert(TK_Add == op || TK_SUB == op);
        double right = CalcExpression(VECTOR(exp->subExp)[i+1]);
        left = (TK_Add == op) ? (left + right) : (left - right);
    }

    return left;
}

double CalcExpression(struct Expression *exp) {
    switch (exp->type) {
    case ET_PrimaryExp_Constant:
        return exp->content.constant;
    case ET_PrimaryExp_Variable:
        return CalcPrimaryExp_Variable(exp);
    case ET_AdditiveExp:
        return CalcAdditiveExp(exp);
    default:
        return 0;
    }
}


struct Parser *CreateParser() {
    struct Parser *re = (struct Parser*)malloc(sizeof(struct Parser));
    re->lexer = 0;
    re->symTable = 0;
    re->ctoken = 0;

    return re;
}

void ParserSetLexer(struct Parser *parser, struct Lexer *lexer) {
    parser->lexer = lexer;
}

void DestroyParser(struct Parser *parser) {
    free(parser);
}

struct Expression *ParserPrimaryExpression(struct Parser *parser) {
    struct Expression *re = 0;
    struct Token *tk = (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;

    if (TK_FloatingConstant == tk->token) {
        re = CreatePrimaryExp_Constant(tk);
    } else if (TK_Id == tk->token) {
        // TODO: TK_Id更换为TK_Variable
        Symbol *sym = InsertSymbol(parser->symTable, tk->str, TK_Variable, 0);
        re = CreatePrimaryExp_Variable(sym);
    }

    DestroyToken(tk);
    return re;
}

struct Expression *ParseExpression(struct Parser *parser) {
    struct Expression *re = ParserPrimaryExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = GetNextToken(parser->lexer);
    while (TK_Add == tk->token || TK_SUB == tk->token) {
        parser->ctoken = 0;
        struct Expression *right = ParserPrimaryExpression(parser);
        assert(0 != right);
        re = CreateAdditiveExp(re, tk->token, right);
        DestroyToken(tk);
        tk = GetNextToken(parser->lexer);
    }
    parser->ctoken = tk;

    return re;
}



