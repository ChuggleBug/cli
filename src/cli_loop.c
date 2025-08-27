
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "cli_config.h"
#include "cli_loop.h"
#include "cli_types.h"

#if !defined(CLI_TEXT_INPUT_STREAM) && !defined(CLI_TEXT_INPUT_PACKET)
#error "At least one of CLI_TEXT_INPUT_STREAM and CLI_TEXT_INPUT_PACKET needs to be defined"
#endif

#if defined(CLI_TEXT_INPUT_STREAM) && defined(CLI_TEXT_INPUT_PACKET)
#error "Both CLI_TEXT_INPUT_STREAM and CLI_TEXT_INPUT_PACKET are defined. Only one can be defined at a time"
#endif


// Because this interface (or at least should be) a single threaded
// environment, only a single copy of these elements need to be stored
// for the active command
static char cmd_str[CMD_STR_BUFZ];
static char *argv_buf[ARGV_MAX];

static int cmd_status;
static bool do_exit = false;

static int exec_cmd(int argc, const char **argv);
static int parse_args(char *str, char **tokens);
static void cli_readline(char *bufptr, size_t cnt);

int exec_from_table(const char* cmd, const cmd_elem_t* table, int table_size, int argc, const char** argv) {
    if (argc == 0) {
        return -1;
    }
    for (int i = 0; i < table_size; i++) {
        if ( ( table[i].alias != NULL && (strcmp(table[i].alias, cmd) == 0) ) || 
                strcmp(table[i].cmd_name, cmd) == 0) {
            if (table[i].command == NULL) {
                break;
            }
            return table[i].command(argc, argv);
        }
    }
    cli_printf("Command %s not found" NEWLINE, cmd);
    return -1;
}

void cli_loop(void) {
    do_exit = false;
    do {
        cli_printf("%s", SHELL_PROMPT);
        cli_readline(cmd_str, CMD_STR_BUFZ);
        int argc = parse_args(cmd_str, argv_buf);
        cmd_status = exec_cmd(argc,(const char**)argv_buf);
    } while (!do_exit);
}


int exec_cmd(int argc, const char **argv) {
    if (argc == 0) {
        return 0;
    } 

    const char *cmd_name = argv[0];

    // Special case for the exit command
    if (strcmp(cmd_name, "exit") == 0) {
        cli_printf("Exiting..." NEWLINE);
        do_exit = true;
        return 0;
    }

    return exec_from_table(cmd_name, command_mapping, command_mapping_size, argc, argv);
}

static inline int is_newline(char c) {
    return (c == '\n') || (c == '\r');
}

static int parse_args(char *str, char **tokens) {
    int argc_cnt = 0;

    while (*str) {
        // Skip leading whitespace
        while (isspace(*str)) str++;

        if (*str == '\0') break;

        // Start of token
        tokens[argc_cnt++] = str;

        // Move until next whitespace or end
        while (*str && !isspace(*str)) str++;

        // Null-terminate the token
        if (*str) {
            *str = '\0';
            str++;
        }
    }

    return argc_cnt;
}

static void cli_readline(char *bufptr, size_t cnt) {
#ifdef CLI_TEXT_INPUT_PACKET  
    while (!cli_read_ready());
    strncpy(bufptr, cli_read_str(), cnt-1);
#endif
#ifdef CLI_TEXT_INPUT_STREAM
    size_t i;
    char c;
    int termlen = strlen(NEWLINE);
    for (i = 0; i < cnt - termlen; i++) {
        while (!cli_read_ready());
        c = cli_read_char();
        bufptr[i] = c;
        if (i >= (termlen - 1) && (strncmp(&(bufptr[i - termlen + 1]), NEWLINE, termlen) == 0)) {
            bufptr[i - termlen + 1] = '\0';
            return;
        }
    }
    bufptr[i] = '\0';
#endif
}