#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

const size_t LINE_LEN_MAX = 1023;

void trim_ending_whitespace(char *input)
{
    char *end = input + strlen(input) - 1;
    while (end > input && isspace((char)*end))
        end--;
    end[1] = '\0';
}

char *trim_whitespace(char *input)
{
    while (isspace((char)*input))
    {
        input++;
    }
    if (*input == 0)
        return input;
    trim_ending_whitespace(input);
    return input;
}

vector split(char *input, char *delim)
{
    vector ret = vector_new(NULL);

    char *saveptr = NULL;
    for (char *str = input;; str = NULL)
    {
        char *token = strtok_r(str, delim, &saveptr);
        if (token == NULL)
        {
            break;
        }
        vector_push(ret, strdup(trim_whitespace(token)));
    }
    return ret;
}

command_t *command_new(vector cmdline, char *redirect)
{
    command_t *ret = (command_t *)malloc(sizeof(command_t));
    ret->cmdline = cmdline;
    ret->redirect = redirect;
    return ret;
}

void command_free(command_t *command)
{
    vector_free(command->cmdline);
    free(command->redirect);
    free(command);
}

char *parse_redirect(char *command)
{
    char *dest = strstr(command, ">>");
    if (dest == NULL)
    {
        return NULL;
    }
    if (dest > command)
    {
        *(dest - 1) = '\0';
    }
    dest += 2;
    return strdup(trim_whitespace(dest));
}

vector parse(char *input)
{
    vector pipe_commands = split(input, "|");
    vector commands = vector_new((vector_item_free_callback)command_free);

    for (size_t i = 0; i < pipe_commands->len; i++)
    {
        char *command = (char *)vector_get(pipe_commands, i);
        char *redirect = parse_redirect(command);
        vector cmdline = split(command, " ");
        vector_push(commands, command_new(cmdline, redirect));
    }
    vector_free(pipe_commands);

    return commands;
}