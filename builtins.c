#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "builtins.h"
#include "parser.h"
#include "vector.h"
#include "utils.h"

typedef void (*builtin_cmd_func)(command_t *);
typedef struct
{
  char *name;
  builtin_cmd_func func;
} builtin_cmd_t;

void builtin_info(command_t *command)
{
  (void)command;
  printf("COMP2211 Simplified Shell by sc18yc\n");
}

void builtin_pwd(command_t *command)
{
  (void)command;
  char *cwd_ptr = getcwd_s();
  if (cwd_ptr == NULL)
  {
    fprintf(stderr, "getcwd() failed");
  }
  else
  {
    printf("%s\n", cwd_ptr);
    free(cwd_ptr);
  }
}

void builtin_cd(command_t *command)
{
  if (command->cmdline->len < 2)
  {
    fprintf(stderr, "cd: no path specified\n");
    return;
  }
  if (command->cmdline->len > 2)
  {
    fprintf(stderr, "cd: too many arguments, only 1 allowed.\n");
    return;
  }
  char *path = (char *)vector_get(command->cmdline, 1);
  if (chdir(path) < 0)
  {
    int err = errno;
    fprintf(stderr, "cd: %s\n", strerror(err));
  }
}

void builtin_exit(command_t *command)
{
  (void)command;
  exit(0);
}

void builtin_mygrep(command_t *command) {}

const builtin_cmd_t builtins[] = {
    {"info", builtin_info},
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit", builtin_exit},
    {"mygrep", builtin_mygrep},
    {NULL, NULL}};

int run_builtin(command_t *command)
{
  int i = 0;
  while (1)
  {
    if (builtins[i].name == NULL)
    {
      break; // Search done, not builtin
    }

    if (strcmp(builtins[i].name, (char *)vector_get(command->cmdline, 0)) == 0)
    {
      builtins[i].func(command);
      return 0;
    }

    i++;
  }

  return -1;
}