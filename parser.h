#pragma once
#include "vector.h"

vector parse(char *input);

typedef struct {
    vector cmdline;
    char* redirect;
} command_t;