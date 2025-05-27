
#include <stdio.h>

#include "cli_loop.h"
#include "cli_config.h"

int help_command(int argc, const char **argv) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: \"%s\"\n", i, argv[i]);
        printf("bytes: ");
        const unsigned char *p = (unsigned char *)argv[i];
        int j = 0;
        while (1) {
            printf("0x%02x ", p[j]);
            if (p[j] == '\0') break;  // stop after null terminator
            j++;
        }
        printf("\n");
    }
    return 0;
}

// Example mappings
const cmd_elem_t command_mapping[] = {
    {"help", 0x0, help_command},
};

const int command_mapping_size = 1;

int main() {
    cli_loop();

    return 0;
}