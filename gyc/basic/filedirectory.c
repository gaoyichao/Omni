#include <filedirectory.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/*
 * CurrentWorkDir - 获取当前工作目录,内存需要释放
 */
char *CurrentWorkDir() {
    size_t size = 128;
    while (1) {
        char *buffer = (char*)malloc(size);
        assert(0 != buffer);

        if (buffer == getcwd(buffer, size))
            return buffer;

        free(buffer);
        if (errno != ERANGE)
            return 0;
        size *= 2;
    }
}
