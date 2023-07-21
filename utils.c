#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "utils.h"

char *getcwd_s()
{
  size_t cwd_len = FILENAME_MAX + 1;
  char *cwd = (char *)malloc(cwd_len);
  char *cwd_ptr = getcwd(cwd, cwd_len);
  return cwd_ptr;
}