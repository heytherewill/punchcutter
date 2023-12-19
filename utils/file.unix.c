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

void createDirectoryIfNeeded(const char *directory) {
    struct stat st = { 0 };
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
    }
}

#endif