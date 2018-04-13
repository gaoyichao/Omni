#ifndef PARSER_H
#define PARSER_H

#include <types.h>
#include <lexer.h>
#include <symboltable.h>

typedef enum eExpType {
    ET_PrimaryExp_Variable = 0,
    ET_PrimaryExp_Constant,
    ET_PrimaryExp_Exp,

    ET_AdditiveExp,
    ET_MultiplicativeExp
} eExpType;

struct Expression;
typedef struct Expression* ExpressionPtr;
#define VECTOR_NUMBER
#define T ExpressionPtr
#define TEMPLATE_TYPE(a) a ##_ExpressionPtr
#define TEMPLATE_FUNCTION(a,b) a ##_ExpressionPtr_## b
#include <vector/vector_t_h.h>
#include <vector/vector_t_num_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

#define IsPrimaryExpression(pexp)    ((ET_PrimaryExp_Constant == (pexp)->type) \
                                   || (ET_PrimaryExp_Exp == (pexp)->type) \
                                   || (ET_PrimaryExp_Variable == (pexp)->type))

#define IsAdditiveExpression(pexp)  ((ET_AdditiveExp == (pexp)->type) || IsPrimaryExpression(pexp))


typedef union uExpContent {
    double constant;
    struct Symbol *variable;
    struct Expression *expression;
} uExpContent;

struct Expression {
    eExpType type;
    uExpContent content;
    vector_eToken operators;
    vector_ExpressionPtr subExp;
};

struct Expression *CreatePrimaryExp_Constant(struct Token *tk);
struct Expression *CreatePrimaryExp_Variable(Symbol *symbol);
double CalcExpression(struct Expression *exp);
void DestroyExpression(struct Expression *exp);

struct Parser {
    struct Lexer *lexer;
    struct SymbolTable *symTable;
    struct Token *ctoken;
};

struct Parser *CreateParser();
void ParserSetLexer(struct Parser *parser, struct Lexer *lexer);
void DestroyParser(struct Parser *parser);
struct Expression *ParseExpression(struct Parser *parser);


#endif // PARSER_H
