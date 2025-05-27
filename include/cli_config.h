#ifndef __CLI_CONFIG_H
#define __CLI_CONFIG_H

#include <stdbool.h>
#include <string.h>

#define SHELL_PROMPT "swd $ "

#define NEWLINE "\n\r"

// Maximum allowable input string when reading a command
#define CMD_STR_BUFZ 256

// Maximum allowable number of arguments for a single command
#define ARGV_MAX 64

// In some systems sending a chararcter to the command line
// automatically echos back the chararcter
#define NEEDS_CHAR_ECHO false

// Overrideable compile time funcions
// writing functions should flush output
// Weak configs are provided for a *nix system
extern void write_str(const char* str, int cnt);
extern void write_char(char c);
extern void write_fmt(const char *fmt, ...);

extern bool read_ready();
extern char read_char();
extern void clear_read_buf();

// Predefined utilities:
// Defined inside of base_configs.c
void writeln(const char* str);

#endif // __CLI_CONFIG_H