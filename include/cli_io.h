
#ifndef __CLI_IO_H
#define __CLI_IO_H

#include <stdbool.h>

#include "cli_config.h"

/*
 * Printf-like interface
 */
__attribute__((format(printf, 1, 2)))
extern void cli_printf(const char* fmt, ...);

#ifdef CLI_TEXT_INPUT_STREAM

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

#endif // __CLI_IO_H