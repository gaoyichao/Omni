#include <inputs.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <error.h>
#include <ctype.h>
#include <mm.h>

/*
 * CreateInputs - 创建一个Input，把给定文件内容全部读到内存中
 *
 * @filename: 文件名称
 */
struct Inputs *CreateInputs(const char *filename) {
    FILE *file = fopen(filename, "r");
    assert(0 != file);

    struct Inputs *inputs = CreateInputsFromFile(file);
    int n = strlen(filename);
    inputs->fileName = malloc(n + 1);
    memcpy(inputs->fileName, filename, n);
    inputs->fileName[n] = '\0';

    return inputs;
}

struct Inputs *CreateInputsFromFile(FILE *file) {
    assert(0 != file);

    struct Inputs *inputs = (struct Inputs *)malloc(sizeof(struct Inputs));
    inputs->file = file;
    inputs->bufsize = InputBufSize;
    inputs->buf = (uint8*)malloc(InputBufSize);
    assert(0 != inputs->buf);
    fgets((char*)(inputs->buf), inputs->bufsize, inputs->file);
    inputs->cursor = inputs->buf;
    inputs->line = 1;
    inputs->col = 1;

    return inputs;
}

void DestroyInputs(struct Inputs *inputs) {
    if (0 != inputs->fileName)
        free(inputs->fileName);
    if (0 != inputs->buf)
        free(inputs->buf);
    fclose(inputs->file);
    free(inputs);
}

uint8 *InputsGets(struct Inputs *inputs) {
    uint8 tmp = inputs->cursor[0];
    uint8 *re = (uint8*)fgets((char*)(inputs->buf), inputs->bufsize, inputs->file);
    if (0 == re)
        return 0;
    inputs->cursor = inputs->buf;
    if ('\n' == tmp) {
        inputs->line++;
        inputs->col = 1;
    }
    return re;
}

static BOOL _InputsExpectUtf8(struct Inputs *inputs) {
    uint8 tmp = inputs->cursor[0];
    inputs->cursor++;
    if ((0x80 & tmp) && (!(0x40 & tmp)))
        return TRUE;
    else
        return FALSE;
}

/*
 * InputsGetNextChar - 获取下一个字节,utf8
 *
 * @inputs: 考察输入序列
 */
uint8 InputsPopChar(struct Inputs *inputs) {
    uint8 tmp = inputs->cursor[0];

    inputs->cursor++;
    if (!(0x80 & tmp)) {
        return tmp;
    }

    if (IsUtf8_2ByteHeader(tmp)) {
        return _InputsExpectUtf8(inputs) ? InputUtf8_2Byte : InputUtf8_Error;
    }

    if (IsUtf8_3ByteHeader(tmp)) {
        for (int i = 0; i < 2; i++) {
            if (!_InputsExpectUtf8(inputs))
                return InputUtf8_Error;
        }
        return InputUtf8_3Byte;
    }

    if (IsUtf8_4ByteHeader(tmp)) {
        for (int i = 0; i < 3; i++) {
            if (!_InputsExpectUtf8(inputs))
                return InputUtf8_Error;
        }
        return InputUtf8_4Byte;
    }

    if (IsUtf8_5ByteHeader(tmp)) {
        for (int i = 0; i < 4; i++) {
            if (!_InputsExpectUtf8(inputs))
                return InputUtf8_Error;
        }
        return InputUtf8_5Byte;
    }

    if (IsUtf8_6ByteHeader(tmp)) {
        for (int i = 0; i < 5; i++) {
            if (!_InputsExpectUtf8(inputs))
                return InputUtf8_Error;
        }
        return InputUtf8_6Byte;
    }
    return InputUtf8_Error;
}


