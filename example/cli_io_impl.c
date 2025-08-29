

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "cli_config.h"
#include "cli_io.h"

void cli_printf(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
    fflush(stdout);
}

#ifdef CLI_TEXT_INPUT_PACKET

char buf[256];
const char* cli_read_str(void) {
    // printf("call read str\n");
    int i = 0;
    char c;
    while (i < 255) {
        c = fgetc(stdin);
        if (c == '\n'){
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

#endif // CLI_TEXT_INPUT_PACKET

#ifdef CLI_TEXT_INPUT_STREAM

char cli_read_char(void) {
    return fgetc(stdin);
}

#endif // CLI_TEXT_INPUT_STREAM

/* 
 * fgetc always blocks so data will always be present when reading
 */ 
bool cli_read_ready(void) {
    return true;
}