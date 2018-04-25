#include <preprocessor.h>
#include <assert.h>
#include <stdlib.h>
#include <mm.h>

struct PreProcessor *CreatePreProcessor() {
    struct PreProcessor *re = Calloc(1, struct PreProcessor);
    re->lexer = 0;
    re->parser = 0;
    re->symTable = 0;

    return re;
}






