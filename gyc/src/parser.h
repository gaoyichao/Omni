#ifndef PARSER_H
#define PARSER_H

#include <types.h>
#include <lexer.h>
#include <expression.h>


struct Parser {
    struct Lexer *lexer;
    struct SymbolTable *symTable;

    int ctkIndex;
    eToken ctk;
};

struct Parser *CreateParser();
void DestroyParser(struct Parser *parser);
struct Expression *ParseExpression(struct Parser *parser);

struct Token *_PeekToken(struct Parser *parser);
struct Token *_NextToken(struct Parser *parser);

#endif // PARSER_H
