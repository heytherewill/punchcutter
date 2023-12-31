#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <cmark.h>
#include <stdio.h>
#include "command.h"
#include "../utils/utils.h"

static long getFileSize(FILE *filePointer) {
    const long originalPosition = ftell(filePointer);
    fseek(filePointer, 0, SEEK_END);
    const long size = ftell(filePointer);
    fseek(filePointer, originalPosition, SEEK_SET);
    return size;
}

static char *readContentsOfFileWithOffset(char *path, long startingIndex) {
    FILE* filePointer = fopen(path, "r");
    const long size = getFileSize(filePointer);
    fseek(filePointer, startingIndex, SEEK_SET);
    const long bytesToRead = size - startingIndex;
    char *fileContents = malloc(bytesToRead * sizeof(char));
    fread(fileContents, sizeof(char), bytesToRead, filePointer);
    fclose(filePointer);
    return fileContents;
}

static char *readContentsOfFile(char *path) {
    return readContentsOfFileWithOffset(path, 0);
}

static char *listItemForNode(
    Node node,
    const char *currentPath,
    char *breadcrumbs[],
    int numberOfBreadcrumbs
) {
    const char *pattern =
        "<li><a class=\"%s\" href=\"%s.html\">%s</a></li>";
    const unsigned long patternLength = strlen(pattern);

    const char *cssClass =
        equals(node.path, currentPath) ? "page" :
        contains(breadcrumbs, numberOfBreadcrumbs, node.path) ? "italic" :  "";
    const unsigned long cssClassLength = strlen(cssClass);

    char *title = nodeTitle(node);
    const unsigned long titleLength = strlen(title);
    const unsigned long pathLength = strlen(node.path);

    const unsigned long finalStringLength = patternLength + cssClassLength + pathLength + titleLength + 1;
    char *finalString = malloc(finalStringLength * sizeof(char));
    sprintf(finalString, pattern, cssClass, node.path, title);
    finalString[finalStringLength - 1] = '\0';

    return finalString;
}

static char *generateNavForNodes(
    char *breadcrumbs[],
    const int numberOfBreadcrumbs,
    char *currentPath,
    const Node *nodeList,
    const int numberOfNodes
) {
    const char *links = "";
    for (int i = 0; i < numberOfNodes; ++i) {
        const Node node = nodeList[i];
        char *linksForNode = listItemForNode(node, currentPath, breadcrumbs, numberOfBreadcrumbs);
        links = concatenate(links, linksForNode);
    }

    const unsigned long resultLength = 9 + strlen(links) + 1;
    char *result = malloc(resultLength * sizeof(char));
    sprintf(result, "<ul>%s</ul>", links);

    return result;
}

static void writeToDirectory( // NOLINT(*-no-recursion)
    char *template,
    char *outputDirectory,
    Node node,
    Node *grandParents,
    const int numberOfGrandparents,
    Node *parents,
    const int numberOfParents,
    Node *siblings,
    const int numberOfSiblings,
    char *breadcrumbs[],
    int numberOfBreadcrumbs
) {
    if (node.isDirectory) {
        int numberOfNextLevelSiblings = 0;
        Node *nextLevelSiblings = malloc((node.directory.numberOfChildren - 1) * sizeof(Node));
        for (int i = 0; i < node.directory.numberOfChildren; ++i) {
            Node childNode = node.directory.children[i];
            if (equals(childNode.path, node.path)) {
                continue;
            }

            nextLevelSiblings[numberOfNextLevelSiblings++] = childNode;
        }

        const int breadcrumbsInNextLevel = numberOfBreadcrumbs + 1;
        char *nextBreadcrumbs[breadcrumbsInNextLevel];
        for (unsigned int i = 0; i < breadcrumbsInNextLevel; i++) {
            nextBreadcrumbs[i] = i < numberOfBreadcrumbs ? breadcrumbs[i] : node.path;
        }

        for (int i = 0; i < node.directory.numberOfChildren; ++i) {
            Node childNode = node.directory.children[i];
            writeToDirectory(
                template,
                outputDirectory,
                childNode,
                parents,
                numberOfParents,
                siblings,
                numberOfSiblings,
                nextLevelSiblings,
                numberOfNextLevelSiblings,
                nextBreadcrumbs,
                breadcrumbsInNextLevel
            );
        }
    } else {
        char *markdown = readContentsOfFileWithOffset(node.filePath, node.file.frontMatterEndIndex);
        unsigned long markdownLength = strlen(markdown);
        char *outputPath = concatenate(concatenate(outputDirectory, node.path), ".html");
        char *content = cmark_markdown_to_html(markdown, markdownLength, 0);

        const bool includeGrandParents = numberOfGrandparents > 0;
        const bool includeParents = numberOfParents > 0;
        const bool includeSiblings = numberOfSiblings > 0;
        const int numberOfNavigationPanels = includeGrandParents + includeParents + includeSiblings;
        Node *navigationLists[3] = {
            includeGrandParents ? grandParents : includeParents ? parents : siblings,
            includeGrandParents && includeParents ? parents : siblings,
            siblings
        };

        const int navigationListsLengths[3] = {
            includeGrandParents ? numberOfGrandparents : includeParents ? numberOfParents : numberOfSiblings,
            includeGrandParents && includeParents ? numberOfParents : numberOfSiblings,
            numberOfSiblings
        };

        char *links = "";
        for (int i = 0; i < numberOfNavigationPanels; ++i) {
            const Node *nodeListToProcess = navigationLists[i];
            char *panel = generateNavForNodes(
                breadcrumbs,
                numberOfBreadcrumbs,
                node.path,
                nodeListToProcess,
                navigationListsLengths[i]
            );

            links = concatenate(links, panel);
            free(panel);
        }

        const char *finalHtml =
            replace(
                replace(
                    replace(
                        replace(
                            template,
                            "{PAGE_EMOJI}",
                            node.file.emoji),
                        "{PAGE_LINKS}",
                        links
                    ),
                    "{PAGE_TITLE}",
                    node.file.title
                ),
                "{PAGE_CONTENT}",
                content
            );

        FILE *filePointer = fopen(outputPath, "w");
        fprintf(filePointer, "%s", finalHtml);
        fclose(filePointer);

        free(markdown);
    }
}

static void recursivelyCopyContentsOf(char *includesDirectory, char *outputDirectory) { // NOLINT(*-no-recursion)
    char *correctIncludesDirectory = ensurePathIsCorrect(includesDirectory);
    char *correctOutputDirectory = ensurePathIsCorrect(outputDirectory);
    createDirectoryIfNeeded(correctOutputDirectory);

    DIR *includesDir = opendir(correctIncludesDirectory);
    struct dirent *fileSystemEntryPointer;
    readAllFilesInPath(fileSystemEntryPointer, includesDir) {
        const char *entryName = fileSystemEntryPointer->d_name;
        if (isSkippableFileEntry(entryName)) {
            continue;
        }

        char *originPath = concatenate(correctIncludesDirectory, entryName);
        char *destinationPath = concatenate(correctOutputDirectory, entryName);
        if (isDirectory(originPath)) {
            recursivelyCopyContentsOf(originPath, destinationPath);
        } else {
            copyFile(originPath, destinationPath);
        }

        free(originPath);
        free(destinationPath);
    }

    closedir(includesDir);
    free(correctIncludesDirectory);
    free(correctOutputDirectory);
}

void generateFiles(
    char *directoryToScan,
    char *outputDirectory,
    char *includesDirectory,
    char *templateFile
) {
    const Node rootNode = scan(directoryToScan);
    char *template = readContentsOfFile(templateFile);

    printf("Generating files.\n");
    Node emptyNodeList[0];
    char *emptyBreadcrumbList[0];

    writeToDirectory(
        template,
        outputDirectory,
        rootNode,
        emptyNodeList,
        0,
        emptyNodeList,
        0,
        emptyNodeList,
        0,
        emptyBreadcrumbList,
        0
    );

    recursivelyCopyContentsOf(includesDirectory, outputDirectory);
    printf("Files generated at %s.\n", outputDirectory);
    free(template);
    freeNode(rootNode);
}