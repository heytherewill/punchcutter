#include "command.h"
#include "../utils/utils.h"

Node directoryNode(char *filePath, char *path, DirectoryMetadata directory) {
    const Node result = { true, filePath, path, NULL, NULL, -1, directory };
    return result;
}

Node fileNode(char *filePath, char *path, FileMetadata file) {
    const Node result = {false, filePath, path, file, NULL, 0 };
    return result;
}

char *nodeTitle(Node node) {
    if (node.isDirectory) {
        for (int i = 0; i < node.directory.numberOfChildren; ++i) {
            Node child = node.directory.children[i];
            if (child.isDirectory) {
                continue;
            }

            if (equals(child.path, node.path)) {
                return child.file.title;
            }
        }
    } else {
        return node.file.title;
    }

    return "";
}

void freeNode(Node node) { // NOLINT(*-no-recursion)
    if (node.isDirectory) {
        for (int i = 0; i < node.directory.numberOfChildren; ++i) {
            freeNode(node.directory.children[i]);
        }

        free(node.directory.children);
    } else {
        free(node.file.title);
        free(node.file.emoji);
        free(node.path);
        free(node.filePath);
    }
}