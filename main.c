#include <stdio.h>
#include <stdbool.h>
#include "utils/utils.h"
#include "command/command.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        char *helpMessage =
            "Punchcutter needs at least 4 arguments in order to properly generate pages.\n"
            "Bear in mind all arguments are positional.\n\n"
            "  0: The directory to scan for .md files.\n"
            "  1: The directory where Punchcutter will output the .html files.\n"
            "  2: The path to the template .html file.\n"
            "  3: A path containing other files that will be copied in the output directory.\n"
            "You can also optionally supply --watch as the 5th argument to regenerate the files\n"
            "as you edit the output directory. This is useful for development mode so you can\n"
            "see what each page looks like without re-running the generation command.\n";

        printf("%s", helpMessage);
        return argc > 1 && equals(argv[1], "--watch") ? 0 : 1;
    }

    char *directoryToScan = argv[1];
    char *outputDirectory = argv[2];
    char *templateFilePath = argv[3];
    char *includesDirectory = argv[4];
    const bool shouldWatch = contains(argv, argc, "--watch");
    char *actualOutputDirectory = ensurePathIsCorrect(outputDirectory);
    createDirectoryIfNeeded(actualOutputDirectory);

    if (!shouldWatch) {
        generateFiles(directoryToScan, actualOutputDirectory, includesDirectory, templateFilePath);
    } else {
        watch(directoryToScan, actualOutputDirectory, includesDirectory, templateFilePath);
    }

    return 0;
}
