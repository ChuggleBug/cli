#ifndef __CLI_CONFIG_H
#define __CLI_CONFIG_H

/*
 * Shell prompt which is printed before each line
 * Default is `"cli default $ "`
 */
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
// #define CLI_TEXT_INPUT_STREAM
/* A input method which transmitts data as data packets (e.g. TCP Packet) */
#define CLI_TEXT_INPUT_PACKET


#ifdef CLI_TEXT_INPUT_STREAM

/* 
 * Termination Sequence to indicate when a stream has stopped 
 * This can be different than NEWLINE to better support 
 * serial communication programs
*/
#define INPUT_STREAM_TERM "\r"

#endif // CLI_TEXT_INPUT_STREAM


#endif // __CLI_CONFIG_H