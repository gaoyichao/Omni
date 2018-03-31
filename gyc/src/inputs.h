#ifndef INPUTS_H
#define INPUTS_H

#include <stdio.h>
#include <types.h>

#define InputBufSize 512

struct Inputs {
    char *fileName;
    FILE *file;
    int line;
    int col;
    int bufsize;
    uint8 *buf;
    uint8 *cursor;
};

#define InputUtf8_1Byte ((uint8)0x80)
#define InputUtf8_2Byte ((uint8)0xC0)
#define InputUtf8_3Byte ((uint8)0xE0)
#define InputUtf8_4Byte ((uint8)0xF0)
#define InputUtf8_5Byte ((uint8)0xF8)
#define InputUtf8_6Byte ((uint8)0xFC)
#define InputUtf8_Error ((uint8)0xFF)

#define IsUtf8_2ByteHeader(c) ((InputUtf8_2Byte == (InputUtf8_2Byte & (c))) && !(0x20 & (c)))
#define IsUtf8_3ByteHeader(c) ((InputUtf8_3Byte == (InputUtf8_3Byte & (c))) && !(0x10 & (c)))
#define IsUtf8_4ByteHeader(c) ((InputUtf8_4Byte == (InputUtf8_4Byte & (c))) && !(0x08 & (c)))
#define IsUtf8_5ByteHeader(c) ((InputUtf8_5Byte == (InputUtf8_5Byte & (c))) && !(0x04 & (c)))
#define IsUtf8_6ByteHeader(c) ((InputUtf8_6Byte == (InputUtf8_6Byte & (c))) && !(0x02 & (c)))

#define END_OF_FILE 255

struct Inputs *CreateInputs(const char *filename);
struct Inputs *CreateInputsFromFile(FILE *file);
void DestroyInputs(struct Inputs *inputs);

uint8 *InputsGets(struct Inputs *inputs);

uint8 InputsPopChar(struct Inputs *inputs);


#endif // INPUTS_H
