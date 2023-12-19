#include <sys/fcntl.h>
#include <stdbool.h>
#include <libgen.h>
#include "utils.h"

#ifndef _WIN32
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#endif

static bool fileExists(const char *path) {
    return access(path, F_OK) == 0;
}

char *removeFileExtension(char *fileName) {
    const int indexOfLastDot = strrchr(fileName, '.') - fileName; // NOLINT(*-narrowing-conversions)
    char *result = substring(fileName, 0, indexOfLastDot);
    return result;
}

char *getFileName(char *path) {
    return strdup(basename(path));
}

void copyFile(const char *fromPath, const char *toPath) {
    if (fileExists(toPath)) {
        remove(toPath);
    }

    const int from = open(fromPath, O_RDONLY);
    const int to = open(toPath, O_WRONLY | O_CREAT | O_EXCL, 0666);

    long readSize;
    char buf[4096];
    while (readSize = read(from, buf, sizeof buf), readSize > 0)
    {
        char *outPointer = buf;
        long writtenSize;

        do {
            writtenSize = write(to, outPointer, readSize);

            if (writtenSize >= 0)
            {
                readSize -= writtenSize;
                outPointer += writtenSize;
            }
        } while (readSize > 0);
    }

    close(to);
    close(from);
}