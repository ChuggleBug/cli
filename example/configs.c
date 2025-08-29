

#include <stdio.h>
#include <stdarg.h>

#include "cli_config.h"

void cli_printf(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
    fflush(stdout);
}

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


char cli_read_char(void) {
    // pritnf("call read char\n");
    return fgetc(stdin);
}

/* 
 * fgetc always blocks so data will always be present when reading
 */ 
bool cli_read_ready(void) {
    return true;
}