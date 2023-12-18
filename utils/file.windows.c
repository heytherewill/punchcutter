#ifdef _WIN32
#include <stdbool.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>

bool isDirectory(char *path) {
    DWORD attributes = GetFileAttributesA(path);
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        return (attributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
    }

    return false;
}
#endif