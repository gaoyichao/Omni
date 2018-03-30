#include <stdarg.h>
#include <stdlib.h>
#include <error.h>

void Error(struct Inputs* inputs, const char *format, ...)
{
    va_list ap;

    if (inputs)
        fprintf(stderr, "(%s,%dL,%dC):", inputs->fileName, inputs->line,inputs->col);

    fprintf(stderr, "error:");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

void Fatal(struct Inputs* inputs, const char *format, ...)
{
    va_list ap;

    if (inputs)
        fprintf(stderr, "(%s,%dL,%dC):", inputs->fileName, inputs->line,inputs->col);

    fprintf(stderr, "fatal:");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    exit(-1);
}

void Warning(struct Inputs* inputs, const char *format, ...)
{
    va_list ap;

    if (inputs)
        fprintf(stderr, "(%s,%dL,%dC):", inputs->fileName, inputs->line,inputs->col);

    fprintf(stderr, "warning:");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    return;
}
