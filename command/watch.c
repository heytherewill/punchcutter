#define DMON_IMPL
#include "../dmon/dmon.h"
#include "command.h"

typedef struct ScanInformation {
    char *directoryToScan;
    char *outputDirectory;
    char *includesDirectory;
    char *templateFile;
} ScanInformation;

static void watch_callback(
    dmon_watch_id watch_id,
    dmon_action action,
    const char* rootdir,
    const char* filepath,
    const char* oldfilepath,
    void* user
) {
    ScanInformation *scanInformation = (ScanInformation*)user;
    generateFiles(
        scanInformation->directoryToScan,
        scanInformation->outputDirectory,
        scanInformation->includesDirectory,
        scanInformation->templateFile
    );
}

void watch(
    char *directoryToWatch,
    char *outputDirectory,
    char *includesDirectory,
    char *templateFile
) {
    const ScanInformation scanInformation = {
        directoryToWatch,
        outputDirectory,
        includesDirectory,
        templateFile
    };

    dmon_init();
    dmon_watch(
        directoryToWatch,
        watch_callback,
        DMON_WATCHFLAGS_RECURSIVE,
        ((void *)&scanInformation)
    );

    printf("Watching for changes at %s\n", directoryToWatch);
    printf("Press enter to stop execution.\n");
    getchar();

    dmon_deinit();
}