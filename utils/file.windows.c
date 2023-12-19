#ifdef _WIN32
#include <stdbool.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

bool isDirectory(char *path) {
    DWORD attributes = GetFileAttributesA(path);
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        return (attributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
    }

    return false;
}

void createDirectoryIfNeeded(const char *directory) {
    struct stat st = { 0 };
    if (stat(directory, &st) == -1) {
        mkdir(directory);
    }
}

#endif