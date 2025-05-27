
#ifdef __APPLE__

#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>
#include <termios.h>
#include <stdarg.h>
#include <stdio.h>

#include "cli_config.h"


void write_char(char c) {
    write(STDOUT_FILENO, &c, 1);
}

void write_str(const char* str, int len) {
    write(STDOUT_FILENO, str, len);
}

void write_fmt(const char *fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    write_str(buffer, strlen(buffer));
}


bool read_ready() {
    fd_set fds;
    struct timeval tv = {0, 0};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char read_char() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

void clear_read_buf() {
    char c;
    while (read_ready()) {
        read(STDIN_FILENO, &c, 1);
        if (c == '\n') break;
    }
}


#endif // __APPLE__ 