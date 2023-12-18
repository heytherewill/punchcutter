#ifndef _WIN32
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define pathSeparator "/"

bool isDirectory(char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return S_ISDIR(path_stat.st_mode) ? true : false;
    }

    return false;
}

char *getFileName(char *path) {
    return strdup(basename(path));
}

void createDirectoryIfNeeded(const char *directory) {
    struct stat st = { 0 };
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }
}

char *removeFileExtension(char *fileName) {
    const int indexOfLastDot = strrchr(fileName, '.') - fileName; // NOLINT(*-narrowing-conversions)
    char *result = substring(fileName, 0, indexOfLastDot);
    return result;
}

#endif