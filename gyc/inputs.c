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
    struct Inputs *inputs = (struct Inputs *)malloc(sizeof(struct Inputs));
    FILE *file = fopen(filename, "r");
    assert(0 != file);
    // 获取文件长度
    inputs->fileName = filename;
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    assert(-1 != len);
    fseek(file, 0, SEEK_SET);
    // 读取文件内容，在文件末尾添加EOF
    inputs->stor_begin = (uint8*)malloc(len+1);
    assert(0 != inputs->stor_begin);
    inputs->cursor = inputs->stor_begin;
    inputs->stor_end = inputs->stor_begin + len+1;
    fread(inputs->stor_begin, 1, len, file);
    *((inputs->stor_end)-1) = END_OF_FILE;

    fclose(file);
    return inputs;
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


