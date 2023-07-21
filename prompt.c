#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "utils.h"

char *prompt()
{
    char *cwd_ptr = getcwd_s();
    if (cwd_ptr == NULL)
    {
        fprintf(stderr, "getcwd() failed");
        return NULL;
    }

    char *prompt_rest = " > ";
    char *prompt_full = (char *)malloc(strlen(cwd_ptr) + strlen(prompt_rest) + 1);
    sprintf(prompt_full, "%s%s", cwd_ptr, prompt_rest);
    free(cwd_ptr);

    char *input = readline(prompt_full);
    free(prompt_full);
    add_history(input);
    return input;
}