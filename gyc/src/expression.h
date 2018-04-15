#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <symboltable.h>


typedef enum eExpType {
    ET_PrimaryExp_Variable = 0,
    ET_PrimaryExp_Constant,
    ET_PrimaryExp_Exp,

    ET_UnaryExp,
    ET_MultiplicativeExp,
    ET_AdditiveExp,

    ET_AssignmentExp
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

#define IsPrimaryOperator(op) ((TK_FloatingConstant == (op)) || \
                               (TK_Id == (op)) || \
                               (TK_Variable == (op)) || \
                               (TK_LParenthesis == (op)))
#define IsUnaryOperator(op) ((TK_Add == (op)) || (TK_Sub == (op)))
#define IsAssignmentOperator(op) ((TK_Assign == (op)))

#define IsPrimaryExpression(pexp)    ((ET_PrimaryExp_Constant == (pexp)->type) || \
                                      (ET_PrimaryExp_Exp == (pexp)->type) || \
                                      (ET_PrimaryExp_Variable == (pexp)->type))
#define IsUnaryExpression(pexp) (ET_UnaryExp == (pexp)->type || IsPrimaryExpression(pexp))
#define IsMultiplicativeExpression(pexp) (ET_MultiplicativeExp == (pexp)->type || IsUnaryExpression(pexp))
#define IsAdditiveExpression(pexp)  ((ET_AdditiveExp == (pexp)->type) || IsMultiplicativeExpression(pexp))
#define IsAssignmentExpression(pexp) ((ET_AssignmentExp == (pexp)->type) || IsAdditiveExpression(pexp))


typedef union uExpContent {
    double constant;
    struct Symbol *variable;
} uExpContent;

struct Expression {
    eExpType type;
    uExpContent content;
    vector_eToken operators;
    vector_ExpressionPtr subExp;
};

struct Expression *CreateExpression(eExpType type);
struct Expression *CreatePrimaryExp_Constant(const char *cstLiter);
struct Expression *CreatePrimaryExp_Variable(Symbol *symbol);
struct Expression *CreatePrimaryExp_Exp(struct Expression *exp);

struct Expression *CreateUnaryExp(eToken op, struct Expression *exp);
struct Expression *CreateBiOperandExp(struct Expression *l, eToken op, struct Expression *r, eExpType type);

double CalcExpression(struct Expression *exp);
void DestroyExpression(struct Expression *exp);

#endif // EXPRESSION_H
