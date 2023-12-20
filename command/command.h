#ifndef PunchcutterCommandHeader
#include <stdbool.h>
#include <stdlib.h>

typedef struct FileMetadata {
    char *title;
    char *emoji;
    long frontMatterEndIndex;
} FileMetadata;

typedef struct DirectoryMetadata {
    struct Node *children;
    int numberOfChildren;
} DirectoryMetadata;

typedef struct Node {
    bool isDirectory;
    char *filePath;
    char *path;
    FileMetadata file;
    DirectoryMetadata directory;
} Node;

Node directoryNode(char *filePath, char *path, DirectoryMetadata directory);
Node fileNode(char *filePath, char *path, FileMetadata file);

char *nodeTitle(Node node);
void freeNode(Node node);

Node scan(char *directoryToScan);

void generateFiles(
    char *directoryToScan,
    char *outputDirectory,
    char *includesDirectory,
    char *templateFile
);

void watch(
    char *directoryToWatch,
    char *outputDirectory,
    char *includesDirectory,
    char *templateFile
);
#endif
