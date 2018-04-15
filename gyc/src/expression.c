#include <expression.h>
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

/*
 * CreateExpression - 构建表达式
 *
 * @type: 表达式类型
 */
struct Expression *CreateExpression(eExpType type) {
    struct Expression *re = Calloc(1, struct Expression);
    re->type = type;
    vector_ExpressionPtr_init(&(re->subExp), 0);
    vector_eToken_init(&(re->operators), 0);

    return re;
}
/*
 * CreatePrimaryExp_Constant - 构建一个常数基本表达式
 *
 * @cstLiter: 常数字符串
 */
struct Expression *CreatePrimaryExp_Constant(const char* cstLiter) {
    struct Expression *re = CreateExpression(ET_PrimaryExp_Constant);
    re->content.constant = atof(cstLiter);
    return re;
}
/*
 * CreatePrimaryExp_Variable - 构建一个变量基本表达式
 */
struct Expression *CreatePrimaryExp_Variable(struct Symbol *symbol) {
    struct Expression *re = CreateExpression(ET_PrimaryExp_Variable);
    re->content.variable = symbol;
    return re;
}
/*
 * CreatePrimaryExp_Exp - 构建一个表达式基本表达式
 */
struct Expression *CreatePrimaryExp_Exp(struct Expression *exp) {
    if (ET_PrimaryExp_Exp == exp->type)
        return exp;
    struct Expression *re = CreateExpression(ET_PrimaryExp_Exp);
    vector_ExpressionPtr_push_back(&(re->subExp), exp);
    return re;
}

/*
 * CreateUnaryExp - 构建一个Unary表达式
 *
 * unary-expression:
 *      primary-expression
 *      unary-operator unary-expression
 *
 * @op: unary-operator
 * @exp: unary-expression
 */
struct Expression *CreateUnaryExp(eToken op, struct Expression *exp) {
    assert(IsUnaryExpression(exp));

    if (TK_Begin == op)
        return exp;

    struct Expression *re = CreateExpression(ET_UnaryExp);
    vector_eToken_push_back(&(re->operators), op);
    vector_ExpressionPtr_push_back(&(re->subExp), exp);

    return re;
}
/*
 * CreateBiOperandExp - 构建一个乘除运算的表达式
 *
 * @l: 左操作数
 * @op: 操作符
 * @r: 右操作数
 * @type: 表达式类型
 */
struct Expression *CreateBiOperandExp(struct Expression *l, eToken op, struct Expression *r, eExpType type) {
    struct Expression *re = 0;

    if (type == l->type) {
        re = l;
    } else {
        re = CreateExpression(type);
        vector_ExpressionPtr_push_back(&(re->subExp), l);
    }
    vector_eToken_push_back(&(re->operators), op);

    if (type == r->type) {
        vector_ExpressionPtr_append(&(r->subExp), &(re->subExp));
        vector_eToken_append(&(r->operators), &(re->operators));
        DestroyExpression(r);
    } else {
        vector_ExpressionPtr_push_back(&(re->subExp), r);
    }
    return re;
}

/*
 * DestroyExpression - 销毁表达式
 */
void DestroyExpression(struct Expression *exp) {
    vector_ExpressionPtr_destroy(&(exp->subExp));
    vector_eToken_destroy(&(exp->operators));

    free(exp);
}

/*
 * CalcPrimaryExp_Variable - 计算变量基本表达式
 */
double CalcPrimaryExp_Variable(struct Expression *exp) {
    struct Symbol *symbol = exp->content.variable;
    if (0 == symbol->vptr)
        return 0.0;
    else
        return *(double*)(symbol->vptr);
}

/*
 * CalcUnaryExp - 计算Unary表达式
 */
double CalcUnaryExp(struct Expression *exp) {
    double value = CalcExpression(VECTOR(exp->subExp)[0]);
    eToken op = VECTOR(exp->operators)[0];
    switch (op) {
    case TK_Add: return value;
    case TK_Sub: return -value;
    default: return 0.0;
    }
}
/*
 * CalcMultiplicativeExp - 计算乘除运算表达式
 */
double CalcMultiplicativeExp(struct Expression *exp) {
    double left = CalcExpression(VECTOR(exp->subExp)[0]);
    int n = vector_eToken_size(&(exp->operators));

    for (int i = 0; i < n; i++) {
        eToken op = VECTOR(exp->operators)[i];
        assert(TK_Mul == op || TK_Div == op);
        double right = CalcExpression(VECTOR(exp->subExp)[i+1]);
        left = (TK_Mul == op) ? (left * right) : (left / right);
    }
    return left;
}
/*
 * CalcAdditiveExp -计算加减运算表达式
 */
double CalcAdditiveExp(struct Expression *exp) {
    double left = CalcExpression(VECTOR(exp->subExp)[0]);
    int n = vector_eToken_size(&(exp->operators));

    for (int i = 0; i < n; i++) {
        eToken op = VECTOR(exp->operators)[i];
        assert(TK_Add == op || TK_Sub == op);
        double right = CalcExpression(VECTOR(exp->subExp)[i+1]);
        left = (TK_Add == op) ? (left + right) : (left - right);
    }

    return left;
}
/*
 * CalcAssignmentExp - 计算赋值表达式
 */
double CalcAssignmentExp(struct Expression *exp) {
    int n = vector_eToken_size(&(exp->operators));
    double right = CalcExpression(VECTOR(exp->subExp)[n]);
    Symbol *sym = 0;

    for (int i = (n-1); i >= 0; i--) {
        struct Expression *var = VECTOR(exp->subExp)[i];
        assert(ET_PrimaryExp_Variable == var->type);
        eToken op = VECTOR(exp->operators)[i];
        assert(TK_Assign == op);

        sym = var->content.variable;
        if (0 == sym->vptr)
            sym->vptr = malloc(sizeof(double));
        *(double*)(sym->vptr) = right;
    }

    if (0 == sym || 0 == sym->vptr)
        return 0;

    return *(double*)(sym->vptr);
}

double CalcExpression(struct Expression *exp) {
    switch (exp->type) {
    case ET_PrimaryExp_Constant:
        return exp->content.constant;
    case ET_PrimaryExp_Variable:
        return CalcPrimaryExp_Variable(exp);
    case ET_PrimaryExp_Exp:
        return CalcExpression(VECTOR(exp->subExp)[0]);
    case ET_UnaryExp:
        return CalcUnaryExp(exp);
    case ET_MultiplicativeExp:
        return CalcMultiplicativeExp(exp);
    case ET_AdditiveExp:
        return CalcAdditiveExp(exp);
    case ET_AssignmentExp:
        return CalcAssignmentExp(exp);
    default:
        return 0;
    }
}

