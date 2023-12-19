#include <stdio.h>
#include <stdbool.h>
#include "utils/utils.h"
#include "command/scanner.h"
#include "command/node.h"
#include "command/generator.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        char *helpMessage =
            "Punchcutter needs at least 4 arguments in order to properly generate pages.\n"
            "Bear in mind all arguments are positional.\n\n"
            "  0: The directory to scan for .md files.\n"
            "  1: The directory where Punchcutter will output the .html files.\n"
            "  2: The path to the template .html file.\n"
            "  3: A path containing other files that will be copied in the output directory.\n";
//            TODO: Uncomment once watching is implemented.
//            "You can also optionally supply --watch as the 5th argument to regenerate the files\n"
//            "as you edit the output directory. This is useful for development mode so you can\n"
//            "see what each page looks like without re-running the command.\n";

        printf("%s", helpMessage);
        return argc > 1 && equals(argv[1], "--watch") ? 0 : 1;
    }

    char *directoryToScan = argv[1];
    char *outputDirectory = argv[2];
    char *templateFilePath = argv[3];
    char *includesDirectory = argv[4];
    const bool shouldWatch = contains(argv, argc, "--watch");

    if (!shouldWatch) {
        char *actualOutputDirectory = ensurePathIsCorrect(outputDirectory);
        createDirectoryIfNeeded(actualOutputDirectory);
        const Node rootNode = scan(directoryToScan);
        generateFiles(rootNode, actualOutputDirectory, includesDirectory, templateFilePath);
        freeNode(rootNode);
        printf("Files generated at %s.\n", actualOutputDirectory);
    } else {
        printf("Watching is not implemented yet.\n");
        return -1;
    }
}
