#include <inputs.h>
#include <lexer.h>
#include <error.h>
#include <assert.h>
#include <mm.h>

#include <stdio.h>

#define IsDigit(c)         ((c) >= '0' && (c) <= '9')
#define IsOctDigit(c)      ((c) >= '0' && (c) <= '7')
#define IsHexDigit(c)      (IsDigit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define IsLetter(c)        (((c) >= 'a' && (c) <= 'z') || ((c) == '_') || ((c) >= 'A' && (c) <= 'Z'))
#define IsUtf8Header(c)          (IsUtf8_2ByteHeader(c) || IsUtf8_3ByteHeader(c) || IsUtf8_4ByteHeader(c) || IsUtf8_5ByteHeader(c) || IsUtf8_6ByteHeader(c))
#define IsLetterOrDigit(c) (IsLetter(c) || IsDigit(c))
//#define ToUpper(c)		   (c & ~0x20)

#define CURSOR      (lexer->inputs->cur)
#define COLUMN      (lexer->inputs->col)

static int EatWhiteSpace(struct Lexer *lexer) {
    int len = 0;
    uint8 c = InputsCurrentChar(lexer->inputs);
    while(c == '\t' || c == ' ' || c == '\n') {
        len++;
        c = InputsNextChar(lexer->inputs);
    }
    return len;
}

static uint8 EatEscapeSequence(struct Lexer *lexer) {
    uint8 c = InputsNextChar(lexer->inputs);

    switch (c) {
    case '\'':
    case '\"':
    case '?':
    case '\\':
    case 'a':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
    case 'v':
        return InputsNextChar(lexer->inputs);
    default:
        break;
    }

    if (IsOctDigit(c)) {
        c = InputsNextChar(lexer->inputs);
        if (IsOctDigit(c)) {
            c = InputsNextChar(lexer->inputs);
            if (IsOctDigit(c)) {
                c = InputsNextChar(lexer->inputs);
            }
        }
        return c;
    }

    if ('x' == c) {
        c = InputsNextChar(lexer->inputs);
        assert(IsHexDigit(c));
        while (IsHexDigit(c)) {
            c = InputsNextChar(lexer->inputs);
        }
    }
    return c;
}

static eToken ScanIdentifier(struct Lexer *lexer) {
    uint8 c = InputsCurrentChar(lexer->inputs);
    while (IsLetterOrDigit(c) || '_' == c || IsUtf8Header(c)) {
        c = InputsNextChar(lexer->inputs);
        if (InputUtf8_Error == c)
            Fatal(lexer->inputs, "不能识别的字符");
    }

    // TODO: keyword

    return TK_Id;
}

static eToken ScanCharLiteral(struct Lexer *lexer) {
    uint8 c = InputsNextChar(lexer->inputs);

    while ('\'' != c) {
        if ('\\' == c)
            c = EatEscapeSequence(lexer);
        else
            c = InputsNextChar(lexer->inputs);
    }
    c = InputsNextChar(lexer->inputs);

    return TK_CharConstant;
}

static eToken ScanStringLiteral(struct Lexer *lexer) {
    uint8 c = InputsNextChar(lexer->inputs);

    while ('\"' != c) {
        if ('\\' == c)
            c = EatEscapeSequence(lexer);
        else
            c = InputsNextChar(lexer->inputs);
    }
    c = InputsNextChar(lexer->inputs);

    return TK_StringLiteral;
}

static eToken ScanFloatLiteral(struct Lexer *lexer) {
    uint8 c = InputsCurrentChar(lexer->inputs);

    if ('.' == c) {
        c = InputsNextChar(lexer->inputs);
        while (IsDigit(c))
            c = InputsNextChar(lexer->inputs);
    }

    if ('e' == c || 'E' == c) {
        c = InputsNextChar(lexer->inputs);
        if ('+' == c || '-' == c)
            c = InputsNextChar(lexer->inputs);
        while (IsDigit(c))
            c = InputsNextChar(lexer->inputs);
    }

    while ('f' == c || 'F' == c || 'l' == c || 'L' == c)
        c = InputsNextChar(lexer->inputs);

    return TK_FloatingConstant;
}

static eToken ScanNumber(struct Lexer *lexer) {
    uint8 c = InputsCurrentChar(lexer->inputs);

    if ('0' == c) {
        c = InputsNextChar(lexer->inputs);
        if ('x' == c || 'X' == c) {
            c = InputsNextChar(lexer->inputs);
            while (IsHexDigit(c))
                c = InputsNextChar(lexer->inputs);
        } else {
            while (IsOctDigit(c))
                c = InputsNextChar(lexer->inputs);
        }
    } else {
        while (IsDigit(c))
            c = InputsNextChar(lexer->inputs);
    }

    if ('.' == c || 'e' == c || 'E' == c)
        return ScanFloatLiteral(lexer);

    while ('u' == c || 'U' == c || 'l' == c || 'L' == c)
        c = InputsNextChar(lexer->inputs);
    return TK_IntegerConstant;
}

static eToken ScanBadChar(struct Lexer *lexer) {
    uint8 c = InputsCurrentChar(lexer->inputs);
    Fatal(lexer->inputs, "非法字符:\\x%x", c);
    c = InputsNextChar(lexer->inputs);
    return TK_BadChar;
}




struct Lexer *CreateLexer(struct Inputs *inputs) {
    struct Lexer *lexer = (struct Lexer *)malloc(sizeof(struct Lexer));
    lexer->inputs = inputs;

    uint8 i = 0;
    for (i = 0; i < END_OF_FILE; i++) {
        if (IsLetter(i) || IsUtf8Header(i))
            lexer->scanners[i] = ScanIdentifier;
        else if (IsDigit(i))
            lexer->scanners[i] = ScanNumber;
        else
            lexer->scanners[i] = ScanBadChar;
    }
    lexer->scanners['_'] = ScanIdentifier;
    lexer->scanners['\''] = ScanCharLiteral;
    lexer->scanners['\"'] = ScanStringLiteral;
    lexer->scanners['.'] = ScanFloatLiteral;

    return lexer;
}

#define SCANNER(plexer) ((plexer)->scanners)

struct Token *GetNextToken(struct Lexer *lexer) {
    InputsUnMark(lexer->inputs);
    EatWhiteSpace(lexer);

    InputsMark(lexer->inputs);
    uint8 c = InputsCurrentChar(lexer->inputs);
    struct Token *re = Calloc(1, struct Token);
    re->line = lexer->inputs->line;
    re->col = lexer->inputs->col;

    eToken tk = SCANNER(lexer)[c](lexer);
    int len = InputsGetMarkedLen(lexer->inputs);
    if (len <= 0)
        return 0;

    re->token = tk;
    re->str = Calloc(len+1, uint8);
    memcpy(re->str, lexer->inputs->mark, len);
    re->str[len] = '\0';

    return re;
}

void DestroyToken(struct Token *token) {
    Free(token->str);
    Free(token);
}

