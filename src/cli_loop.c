
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "cli_config.h"
#include "cli_loop.h"
#include "cli_types.h"

// Because this interface (or at least should be) a single threaded
// environment, only a single copy of these elements need to be stored
// for the active command
static char cmd_str[CMD_STR_BUFZ];
static char *argv_buf[ARGV_MAX];

static int cmd_status;
static bool do_exit = false;

static int exec_cmd(int argc, const char **argv);
static char* readline(char *buf, int bufsz);
static int parse_args(char *str, char **tokens);

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
    write_fmt("Command %s not found\n\r", cmd);
    return -1;
}

void cli_loop(void) {
    do_exit = false;
    do {
        write_str(SHELL_PROMPT, strlen(SHELL_PROMPT));
        readline(cmd_str, CMD_STR_BUFZ);
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
        writeln("Exiting...");
        do_exit = true;
        return 0;
    }

    return exec_from_table(cmd_name, command_mapping, command_mapping_size, argc, argv);
}

static inline int is_newline(char c) {
    return (c == '\n') || (c == '\r');
}

char* readline(char *buf, int bufsz){
    while (!read_ready());
    char c = read_char();
    int rd_cnt = 0;
    while (rd_cnt < bufsz - 1) {
        if (is_newline(c)) {
            write_str("\n\r", 2);
            clear_read_buf();
            break;
        }
        else if (c == '\b') {
            if (rd_cnt > 0) {
                rd_cnt--;
                write_str("\b \b", 3);
            }
        } else {
            write_char(c);
            buf[rd_cnt++] = c;
        }
        while (!read_ready());
        c = read_char();
    }
    buf[rd_cnt] = '\0';
    return buf;
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