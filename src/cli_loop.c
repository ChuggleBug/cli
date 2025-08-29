
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "cli_config.h"
#include "cli_loop.h"
#include "cli_types.h"
#include "cli_io.h"


#if !defined(CLI_TEXT_INPUT_STREAM) && !defined(CLI_TEXT_INPUT_PACKET)
#error "At least one of CLI_TEXT_INPUT_STREAM and CLI_TEXT_INPUT_PACKET needs to be defined"
#endif

#if defined(CLI_TEXT_INPUT_STREAM) && defined(CLI_TEXT_INPUT_PACKET)
#error "Both CLI_TEXT_INPUT_STREAM and CLI_TEXT_INPUT_PACKET are defined. Only one can be defined at a time"
#endif

#define DELIMS " \r\n\t\v\f"

__attribute__((weak))
const char *SHELL_PROMPT = "CLI default $ ";

__attribute__((weak))
const cmd_err_elem_t command_err_table[] = {
    {0, 0}
};

// Because this interface (or at least should be) a single threaded
// environment, only a single copy of these elements need to be stored
// for the active command
static char cmd_str[CMD_STR_BUFZ];
static char *argv_buf[ARGV_MAX];

static bool do_exit = false;

static int exec_cmd(int argc, const char **argv);

/*
* @brief Parse an input string into in arg argc argv-like like interface.
*          Sets the first argument of arg_buf to the character which is not inside
*          the delimeter set. In addition to this, the provided string buffer is
*          modified to so that each "word" ends with a null terminator and the reference
*          of its starting character is stored inside of argv (in order).
* @return int The number of arguments parsed. -1 on error
* @note The string buffer needs to remain in scope as long as argv is intended to be used
* 
*/
static int parse_args(char *str_buf, char **argv, int argv_max, const char* delims);

static int strcontains(char ch, const char* charset);
static void cli_readline(char *bufptr, size_t cnt);

int exec_from_table(const char* cmd, const cmd_elem_t* table, int argc, const char** argv) {
    if (cmd == NULL) {
        return 0;
    }
    if (table == NULL) {
        return 0;
    }
    if (argc == 0) {
        return 0;
    }
    if (argv == NULL) {
        return 0;
    }
    for (const cmd_elem_t *current = table; current->cmd_name != 0; current++) {
        if ( ( current->alias != NULL && (strcmp(current->alias, cmd) == 0) ) || 
                strcmp(current->cmd_name, cmd) == 0) {
            if (current->command == NULL) {
                break;
            }
            return current->command(argc, argv);
        }
    }
    cli_printf("Command not found: %s" NEWLINE, cmd);
    return 0;
}

void cli_loop(void) {
    int argc;
    do_exit = false;
    do {
        cli_printf("%s", SHELL_PROMPT);
        cli_readline(cmd_str, CMD_STR_BUFZ);
        argc = parse_args(cmd_str, argv_buf, ARGV_MAX, DELIMS);
        exec_cmd(argc,(const char**)argv_buf);
    } while (!do_exit);
}


int exec_cmd(int argc, const char **argv) {
    const char *cmd_name;
    int ret;

    if (argc == 0) {
        return 0;
    } 

    cmd_name = argv[0];

    // Special case for the exit command
    if (strcmp(cmd_name, "exit") == 0) {
        cli_printf("Exiting..." NEWLINE);
        do_exit = true;
        return 0;
    }

    ret = exec_from_table(cmd_name, command_table, argc, argv);
    if (ret != 0) {
        cli_printf("%s: ", cmd_name);
        for (const cmd_err_elem_t *current = command_err_table; current->err_no != 0 && current->err_str != 0; current++) {
            if (current->err_no == ret) {
                cli_printf("%s" NEWLINE, current->err_str);
                return ret;
            }
        }
        cli_printf("Unknown error %d" NEWLINE, ret);
    }
    return ret;
}

static int strcontains(char ch, const char* charset) {
    const char *cp = charset;
    char test;
    while ( (test = *(cp++)) != '\0') {
        if (ch == test) {
            return 1;
        }
    } 
    return 0;
}

static int parse_args(char *str_buf, char **argv, int argv_max, const char* delims) {
    char *cptr = str_buf;
    char *arg_start = NULL;
    int argc = 0;

    if (str_buf == NULL || argv == NULL || 
        delims == NULL || argv_max <= 0) 
    {
        return -1;
    }

    while (*cptr != '\0') {
        // Stop parsing if no more arguments can be placed in argv
        if (argc >= argv_max) {
            return argv_max;
        }

        // Skip delimiters
        while (strcontains(*cptr, delims)) {
            cptr++;
        }
        
        // Check if not more characters are present
        if (*cptr == '\0') {
            break; 
        }

        arg_start = cptr;

        // Find where "word" ends (or if the null terminator is)
        while (*cptr != '\0' && !strcontains(*cptr, delims)) {
            cptr++;
        }

        argv[argc++] = arg_start;

        // Create "word" for argv to use
        if (*cptr != '\0') {
            *(cptr++) = '\0';
        }
    }

    return argc;
}

static void cli_readline(char *bufptr, size_t cnt) {
#ifdef CLI_TEXT_INPUT_PACKET  
    while (!cli_read_ready());
    strncpy(bufptr, cli_read_str(), cnt-1);
#endif
#ifdef CLI_TEXT_INPUT_STREAM
    size_t i;
    char c;
    int termlen = strlen(INPUT_STREAM_TERM);
    for (i = 0; i < cnt - termlen; i++) {
        while (!cli_read_ready());
        c = cli_read_char();
        bufptr[i] = c;
        if (i >= (termlen - 1) && (strncmp(&(bufptr[i - termlen + 1]), INPUT_STREAM_TERM, termlen) == 0)) {
            bufptr[i - termlen + 1] = '\0';
            return;
        }
    }
    bufptr[i] = '\0';
#endif
}