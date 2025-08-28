#ifndef __CLI_CONFIG_H
#define __CLI_CONFIG_H

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

extern const char *SHELL_PROMPT;

#define NEWLINE "\n"

// Maximum allowable input string when reading a command
#define CMD_STR_BUFZ 1024

// Maximum allowable number of arguments for a single command
#define ARGV_MAX 64

/*
 * Two options are provided to determine how user inputs arrives
 * for the CLI to process. Both options should be mutually exclusive
 */

/* A input method which transmitts data as a input steam (e.g. UART) */
#define CLI_TEXT_INPUT_STREAM
/* A input method which transmitts data as data packets (e.g. TCP Packet) */
// #define CLI_TEXT_INPUT_PACKET

/*
 * Printf-like interface
 */
extern void cli_printf(const char* fmt, ...);

#ifdef CLI_TEXT_INPUT_STREAM

/* 
 * Termination Sequence to indicate when a stream has stopped 
 * This can be different than NEWLINE to better support 
 * serial communication programs
*/
#define INPUT_STREAM_TERM "\r"

/*
 * Returns a single character for the CLI to use internally.
 * CLI reads up to CMD_STR_BUFZ-strlen(INPUT_STREAM_TERM) characters or up until a
 * null terminator
 */
extern char cli_read_char(void);

#endif // CLI_TEXT_INPUT_STREAM

#ifdef CLI_TEXT_INPUT_PACKET

/*
 * Returns a single string for the CLI to use internally.
 * CLI copies up to CMD_STR_BUFZ-1 characters 
 */
extern const char* cli_read_str(void);

#endif // CLI_TEXT_INPUT_PACKET

/*
 * Flag to determine whether or not data is present
*/
extern bool cli_read_ready();

#endif // __CLI_CONFIG_H