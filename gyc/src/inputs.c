#include <inputs.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <error.h>
#include <ctype.h>
#include <mm.h>

/*
 * CreateInputs - 创建一个Input，打开文件逐行读取数据
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
/*
 * CreateInputsFromFile - 创建一个Input，
 *
 * @file: 文件指针
 */
struct Inputs *CreateInputsFromFile(FILE *file) {
    assert(0 != file);

    struct Inputs *inputs = (struct Inputs *)malloc(sizeof(struct Inputs));
    inputs->file = file;
    inputs->buf = (uint8*)malloc(InputBufSize);
    assert(0 != inputs->buf);
    inputs->buf_end = inputs->buf + InputBufSize;
    fgets((char*)(inputs->buf), InputReadLen, inputs->file);
    inputs->cur = inputs->buf;
    inputs->end = inputs->buf + strlen((char*)(inputs->cur));
    inputs->mark = inputs->end;
    inputs->line = 1;
    inputs->col = 1;

    return inputs;
}
/*
 * DestroyInputs - 销毁Input
 *
 * @inputs: 目标
 */
void DestroyInputs(struct Inputs *inputs) {
    if (0 != inputs->fileName)
        free(inputs->fileName);
    if (0 != inputs->buf)
        free(inputs->buf);
    fclose(inputs->file);
    free(inputs);
}
/*
 * InputsMark - 标记当前位置
 */
void InputsMark(struct Inputs *inputs) {
    inputs->mark = inputs->cur;
}
/*
 * InputsUnMark - 取消标记
 */
void InputsUnMark(struct Inputs *inputs) {
    inputs->mark = inputs->end;
}
/*
 * InputsIsMarked - 判定当前是否标记
 */
BOOL InputsIsMarked(struct Inputs *inputs) {
    return (inputs->mark != inputs->end);
}
/*
 * InputsGetMarkedLen - 获取当前位置到标记间的长度
 */
int InputsGetMarkedLen(struct Inputs *inputs) {
    return (inputs->cur - inputs->mark);
}
/*
 * InputsRead - 读入数据
 */
static void _InputsRead(struct Inputs *inputs) {
    BOOL isMarked = InputsIsMarked(inputs);
    uint8 *start = isMarked ? inputs->mark : inputs->cur;
    int costed = isMarked ? (inputs->cur - inputs->mark) : 0;
    int left = inputs->end - start;
    int cap = inputs->buf_end - inputs->buf;
    int need = left + InputReadLen;
    uint8 *buf = inputs->buf;

    if (cap < need)
        buf = (uint8*)malloc(2 * need);

    if (left > 0) {
        memmove(buf, start, left);
        if (buf != inputs->buf) {
            free(inputs->buf);
            inputs->buf = buf;
            inputs->buf_end = inputs->buf + (2 * need);
        }
        inputs->end = inputs->buf + left;
        inputs->mark = isMarked ? inputs->buf : inputs->end;
        inputs->cur = inputs->buf + costed;
    }

    if (0 == fgets((char*)inputs->end, InputReadLen, inputs->file)) {
        inputs->end[0] = END_OF_FILE;
        return;
    }
    inputs->end += strlen((char*)(inputs->end));
}
/*
 * _InputsRepairBuf - 读入更多的数据
 */
static void _InputsRepairBuf(struct Inputs *inputs) {
    if (END_OF_FILE == inputs->end[0])
        return;
    int len = inputs->end - inputs->cur;
    if (len < 2)
        _InputsRead(inputs);
}
/*
 * _InputsExpectUtf8 - 断言当前字节是utf8编码的一个字节，并跳过
 */
static BOOL _InputsExpectUtf8(struct Inputs *inputs) {
    _InputsRepairBuf(inputs);

    uint8 tmp = inputs->cur[0];
    inputs->cur++;
    if ((0x80 & tmp) && (!(0x40 & tmp)))
        return TRUE;
    else
        return FALSE;
}

/*
 * InputsGetNextChar - 获取当前字符,并跳过,utf8
 *
 * @inputs: 考察输入序列
 */
uint8 InputsPopChar(struct Inputs *inputs) {
    uint8 tmp = inputs->cur[0];

    inputs->cur++;
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
/*
 * InputsCurrentChar - 获取当前字符,不跳过,utf8
 */
uint8 InputsCurrentChar(struct Inputs *inputs) {
    _InputsRepairBuf(inputs);

    return inputs->cur[0];
}
/*
 * InputsNextChar - 获取下一个字符,utf8
 */
uint8 InputsNextChar(struct Inputs *inputs) {
    uint8 c = InputsPopChar(inputs);
    if (InputUtf8_Error == c)
        return c;

    if ('\n' == c) {
        inputs->line++;
        inputs->col = 1;
    } else {
        inputs->col++;
    }

    return InputsCurrentChar(inputs);
}

