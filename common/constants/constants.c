#include "constants.h"

const char *operation_names[] =
    {
        "EXIT",
        "CREATE",
        "WRITE",
        "APPEND",
        "READ",
        "DELETE"};

const char *operation_descriptions[] =
    {
        "Exit the program",
        "Create a new file",
        "Write to a file",
        "Append to a file",
        "Read a file",
        "Delete a file"};

const char forbidden_chars[] = {
    '?',
    '*',
    '|',
    '<',
    '>',
    ':',
    '"',
    '\\',
    '\0'};