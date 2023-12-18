#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Compare string contents instead of their pointers.
bool equals(const char *lhs, const char *rhs) {
    return strcmp(lhs, rhs) == 0;
}

char *concatenate(const char *lhs, const char *rhs) {   // Calculate the total length needed for the new string
    const unsigned long lhsLength = strlen(lhs);
    const unsigned long rhsLength = strlen(rhs);
    const unsigned long newStringLength = lhsLength + rhsLength + 1;

    // Allocate memory for the new concatenated string.
    char* result = malloc(newStringLength * sizeof(char));

    // Copy the first string into the result buffer
    strcpy(result, lhs);

    // Concatenate the second string to the result buffer
    strcat(result, rhs);

    return result;
}

char *substring(char *baseString, int startingIndex, int length) {
    char *result = (char*)malloc((length + 1) * sizeof(char));
    strncpy(result, baseString + startingIndex, length);
    result[length] = '\0';

    return result;
}

bool startsWith(char *string, char *stringToVerify) {
    const unsigned long lengthOfBaseString = strlen(string);
    const unsigned long lengthOfStringToVerify = strlen(stringToVerify);
    if (lengthOfStringToVerify > lengthOfBaseString) {
        return false;
    }

    const char *beginningOfBaseString = substring(
            string,
            0,
            lengthOfStringToVerify  // NOLINT(*-narrowing-conversions)
    );
    return equals(beginningOfBaseString, stringToVerify);
}

bool endsWith(char *string, char *stringToVerify) {
    const unsigned long lengthOfBaseString = strlen(string);
    const unsigned long lengthOfStringToVerify = strlen(stringToVerify);
    if (lengthOfStringToVerify > lengthOfBaseString) {
        return false;
    }

    const char *endOfBaseString = substring(
            string,
            lengthOfBaseString - lengthOfStringToVerify, // NOLINT(*-narrowing-conversions)
            lengthOfStringToVerify  // NOLINT(*-narrowing-conversions)
    );
    return equals(endOfBaseString, stringToVerify);
}

char *replace(char *baseString, char *old, char *new) {
    const char* occurrence = strstr(baseString, old);

    if (occurrence == NULL) {
        return strdup(baseString);
    }

    unsigned long oldLength = strlen(old);
    unsigned long newLength = strlen(new);
    unsigned long baseStringLength = strlen(baseString);

    unsigned long resultLength = baseStringLength - oldLength + newLength + 1;

    char* result = (char*)malloc(resultLength);
    strncpy(result, baseString, occurrence - baseString);
    strcpy(result + (occurrence - baseString), new);
    strcpy(result + (occurrence - baseString) + newLength, occurrence + oldLength);

    return replace(result, old, new);
}