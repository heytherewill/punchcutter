#ifndef PunchcutterUtilsHeader
#define PunchcutterUtilsHeader

#include "string.h"
#include <stdbool.h>

// Array
bool contains(char *array[], int arraySize, const char *item);

// File
bool isDirectory(char *path);
char *getFileName(char *path);
char *removeFileExtension(char *fileName);
void createDirectoryIfNeeded(const char *directory);
void copyFile(const char *fromPath, const char *toPath);
#define readAllFilesInPath(fileSystemEntryPointer, path) while ((fileSystemEntryPointer = readdir(path)) != NULL)
#ifdef _WIN32
#define pathSeparator "\\"
#else
#define pathSeparator "/"
#endif

#define ensurePathIsCorrect(path) (endsWith(path, pathSeparator) ? strdup(path) : concatenate(path, pathSeparator))
#define isSkippableFileEntry(name) (equals(name, ".DS_Store") || equals(name, ".") || equals(name, ".."))

// String
bool equals(const char *lhs, const char *rhs);
char *concatenate(const char *lhs, const char *rhs);
bool startsWith(char *string, char *stringToVerify);
bool endsWith(char *string, char *stringToVerify);
char *substring(char *baseString, int startingIndex, int length);
char *replace(char *baseString, char *old, char *new);

#endif //PunchcutterUtilsHeader
