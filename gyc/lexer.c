#include <inputs.h>
#include <lexer.h>
#include <error.h>
#include <mm.h>

#define IsDigit(c)         ((c) >= '0' && (c) <= '9')
#define IsOctDigit(c)      ((c) >= '0' && (c) <= '7')
#define IsHexDigit(c)      (IsDigit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define IsLetter(c)        (((c) >= 'a' && (c) <= 'z') || ((c) == '_') || ((c) >= 'A' && (c) <= 'Z'))
#define IsUtf8Header(c)          (IsUtf8_2ByteHeader(c) || IsUtf8_3ByteHeader(c) || IsUtf8_4ByteHeader(c) || IsUtf8_5ByteHeader(c) || IsUtf8_6ByteHeader(c))
#define IsLetterOrDigit(c) (IsLetter(c) || IsDigit(c))
//#define ToUpper(c)		   (c & ~0x20)

#define CURSOR      (lexer->inputs->cursor)
#define COLUMN      (lexer->inputs->col)

static void EatWhiteSpace(struct Lexer *lexer) {
    while(CURSOR[0] == '\t' || CURSOR[0] == ' ') {
        CURSOR++;
    }
}

static eToken ScanIdentifier(struct Lexer *lexer) {
    uint8 pop = 0;
    while (IsLetterOrDigit(CURSOR[0]) || '_' == CURSOR[0] || IsUtf8Header(CURSOR[0])) {
        pop = InputsPopChar(lexer->inputs);
        if (InputUtf8_Error == pop)
            Fatal(lexer->inputs, "不能识别的字符");
    }

    // TODO: keyword

    return TK_ID;
}

static eToken ScanBadChar(struct Lexer *lexer) {
    Fatal(lexer->inputs, "非法字符:\\x%x", CURSOR[0]);
    CURSOR++;
    return TK_BADCHAR;
}

struct Lexer *CreateLexer(struct Inputs *inputs) {
    struct Lexer *lexer = (struct Lexer *)malloc(sizeof(struct Lexer));
    lexer->inputs = inputs;

    uint8 i = 0;
    for (i = 0; i < END_OF_FILE; i++) {
        if (IsLetter(i) || '_' == i || IsUtf8Header(i))
            lexer->scanners[i] = ScanIdentifier;
        else
            lexer->scanners[i] = ScanBadChar;
    }

    return lexer;
}

#define SCANNER(plexer) ((plexer)->scanners)


struct Token *GetNextToken(struct Lexer *lexer) {
    EatWhiteSpace(lexer);

    uint8 *start = CURSOR;
    eToken tk = SCANNER(lexer)[CURSOR[0]](lexer);
    int len = CURSOR - start;

    if (len <= 0)
        return 0;

    struct Token *re = Calloc(1, struct Token);
    re->token = tk;
    re->str = Calloc(len+1, uint8);
    memcpy(re->str, start, len);
    re->str[len] = '\0';

    return re;
}

