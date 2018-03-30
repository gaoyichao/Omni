#ifndef ERROR_H
#define ERROR_H

#include <inputs.h>


void Error(struct Inputs* inputs, const char *format, ...);
void Fatal(struct Inputs* inputs, const char *format, ...);
void Warning(struct Inputs* inputs, const char *format, ...);


#endif // ERROR_H
