#include <stdbool.h>
#include "utils.h"

bool contains(char *array[], const int arraySize, const char *item) {
    int i = 0;
    bool doesContain = false;
    while (i < arraySize && !doesContain) {
        const char *currentItem = array[i++];
        doesContain |= equals(currentItem, item);
    }

    return doesContain;
}