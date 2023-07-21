#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "prompt.h"
#include "parser.h"
#include "builtins.h"

void redirect(int from, int to)
{
    if (from != to)
    {
        dup2(from, to);
        close(from);
    }
}

char **command_to_cmdline(command_t *command)
{
    size_t cmdline_len = command->cmdline->len;
    char **cmdline = (char **)malloc(sizeof(char **) * (cmdline_len + 1));
    for (size_t i = 0; i < cmdline_len; i++)
    {
        cmdline[i] = (char *)vector_get(command->cmdline, i);
    }
    cmdline[cmdline_len] = (char *)NULL;
    return cmdline;
}

pid_t spawn_external_command(int in, int out, command_t *command)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child, set up pipes
        redirect(in, STDIN_FILENO);
        redirect(out, STDOUT_FILENO);

        char **cmdline = command_to_cmdline(command);

        if (strcmp(cmdline[0], "ex") == 0)
        {
            cmdline += 1;
            if (execvp(cmdline[0], cmdline) < 0)
            {
                int err = errno;
                fprintf(stderr, "Failed to spawn %s: %s\n", cmdline[0], strerror(err));
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Unrecognized command \"%s\", builtins are not currently pipeable.\n", cmdline[0]);
            exit(0);
        }
    }
    return pid;
}

void run_external_commands(vector commands)
{
    int in, fd[2];
    pid_t pid;
    int orig_in = dup(STDIN_FILENO);

    for (size_t i = 0; i < commands->len - 1; i++)
    {
        pipe(fd);

        command_t *command = (command_t *)vector_get(commands, i);
        spawn_external_command(in, fd[1], command);

        close(fd[1]); // This end is not used
        in = fd[0];   // Use out of last one as in
    }
    redirect(in, STDIN_FILENO);

    // Spawn the last one
    command_t *command = (command_t *)vector_get(commands, commands->len - 1);
    pid = spawn_external_command(in, fd[1], command);
    // Wait for it to die
    int werror;
    waitpid(pid, &werror, 0);
    // Resotre stdin
    redirect(orig_in, STDIN_FILENO);
}

void run_commands(vector commands)
{
    if (commands->len == 0)
        return;

    if (commands->len == 1)
    {
        // Handle builtin
        command_t *command = (command_t *)vector_get(commands, 0);
        char *command_s = (char *)vector_get(command->cmdline, 0);
        if (strcmp(command_s, "ex") != 0)
        {
            if (run_builtin(command) < 0)
            {
                fprintf(stderr, "Failed to locate command %s\n", command_s);
            }
            return;
        }
    }

    run_external_commands(commands);
}

int main(void)
{
    while (1)
    {
        char *lineinput = prompt();
        if (lineinput == NULL)
        {
            break;
        }
        vector commands = parse(lineinput);
        run_commands(commands);
        free(lineinput);
    }
    return 0;
}