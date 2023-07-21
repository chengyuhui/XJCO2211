# XJCO2211
## What's inside this repo
### `vector.c/.h`
A simple but useful implementation of dynamically growable arrays, similar to C++'s `std::vector`.

### `utils.c/.h`
Just one convenience function that executes `getcwd`.

### `prompt.c/.h`
Handling of shell prompt and history, based on GNU Readline.

### `parser.c/.h`
Parser of shell commands, including handling for piping (`|`), redirection (`>/>>`), single and double quotes, and whitespaces.

### `builtins.c/.h`
Implementation of built-in commands:
* `info`
* `pwd`
* `cd`
* `exit`
* `mygrep`

### `main.c`
Main loop, command spawning, piping, and redirection.
