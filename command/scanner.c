#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "../utils/utils.h"

static char *extractValueForKey(char *metadataEntry, char *searchKey) {

    const int keyLength = strlen(searchKey); // NOLINT(*-narrowing-conversions)
    if (!startsWith(metadataEntry, searchKey)) {
        return NULL;
    }
    const int metadataEntryLength = strlen(metadataEntry); // NOLINT(*-narrowing-conversions)
    char *result = substring(
        metadataEntry,
        keyLength,
        metadataEntryLength - keyLength
    );
    return result;
}

#define coalesceNull(value, defaultValue) (value != NULL ? value : defaultValue)

static FileMetadata readMetadataFromFile(const char *filePath) {

    FILE* filePointer = fopen(filePath, "r");

    char *titleKey = "title: ";
    char *emojiKey = "emoji: ";
    char *title = NULL;
    char *emoji = NULL;

    int numberOfDashesFoundInARow = 0;
    bool beginningDelimiterFound = false;
    bool endDelimiterFound = false;
    bool chainingDashes = false;
    int i = 0;
    int metadataEntryStartIndex = -1;

    do {
        const int currentIndex = i++;
        fseek(filePointer, currentIndex, SEEK_SET);
        const char characterBeingParsed = getc(filePointer); // NOLINT(*-narrowing-conversions)
        if (characterBeingParsed == '-') {
            chainingDashes = true;
            numberOfDashesFoundInARow += 1;
            if (numberOfDashesFoundInARow == 3) {
                if (beginningDelimiterFound) {
                    endDelimiterFound = true;
                } else {
                    beginningDelimiterFound = true;
                    continue;
                }
            }
        } else {
            chainingDashes = false;
        }

        if (!beginningDelimiterFound) {
            continue;
        }

        if (metadataEntryStartIndex == -1) {
            metadataEntryStartIndex = currentIndex;
        }

        numberOfDashesFoundInARow = chainingDashes ? numberOfDashesFoundInARow : 0;

        if (characterBeingParsed == '\n') {
            const int length = currentIndex - metadataEntryStartIndex;
            char *line = malloc((length +1) * sizeof(char));
            for (int j = 0; j < length; ++j) {
                fseek(filePointer, metadataEntryStartIndex + j, SEEK_SET);
                line[j] = getc(filePointer); // NOLINT(*-narrowing-conversions)
            }
            line[length] = '\0';

            char *potentialTitle = extractValueForKey(line, titleKey);
            if (potentialTitle != NULL) {
                title = potentialTitle;
            }

            char *potentialEmoji = extractValueForKey(line, emojiKey);
            if (potentialEmoji != NULL) {
                emoji = potentialEmoji;
            }

            metadataEntryStartIndex = -1;
            free(line);
        }

    } while (!endDelimiterFound && (title == NULL || emoji == NULL));

    const FileMetadata info = {
            coalesceNull(title, ""),
            coalesceNull(emoji, "👋🏽"),
    };

    return info;
}

static Node recursiveScan(char *path) {
    if (isDirectory(path)) {
        int numberOfChildren = 0;
        Node *children = malloc(1000 * sizeof(Node));
        DIR *currentPathDir = opendir(path);
        struct dirent *fileSystemEntryPointer;

        const char *correctPath = ensurePathIsCorrect(path);

        readAllFilesInPath(fileSystemEntryPointer, currentPathDir) {
            char *name = fileSystemEntryPointer->d_name;
            if (isSkippableFileEntry(name)) {
                continue;
            }

            char *childPath = concatenate(correctPath, name);
            children[numberOfChildren++] = recursiveScan(childPath);
        }
        closedir(currentPathDir);

        char *nodePath = getFileName(path);
        const DirectoryMetadata directory = { children, numberOfChildren };
        return directoryNode(path, nodePath, directory);
    } else {
        char *fileName = getFileName(path);
        char *nodePath = removeFileExtension(fileName);
        const FileMetadata file = readMetadataFromFile(path);
        return fileNode(path, nodePath, file);
    }
}

Node scan(char *path) {
    Node rootNode = recursiveScan(path);
    rootNode.path = "index";
    return rootNode;
}