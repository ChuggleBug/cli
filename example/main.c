
#include <stdio.h>

#include "cli_loop.h"
#include "cli_types.h"
#include "cli_config.h"
#include "cli_io.h"

const char *SHELL_PROMPT = "myCLI % ";

int main();
int help_command(int argc, const char **argv);
int error_command(int argc, const char **argv);

enum CMD_ERROR {
    CLI_OK = 0,
    CLI_ETEST_ONE = 1,
    CLI_ETEST_TWO = 2,
     // This enum value is not mapped to the error table
    CLI_ETEST_NOT_MAPPED = 72,
};

// Example Command Error Mappings
// Note that CLI_ETEST_NOT_MAPPED is not in this table
const cmd_err_elem_t command_err_table[] = {
    {CLI_ETEST_ONE, "Test Error One"},
    {CLI_ETEST_ONE, "Test Error Two"},
    {0, 0}
};

// Example Command Mappings
// Note that "error" has "err" as an alias
const cmd_elem_t command_table[] = {
    {"help", 0x0, help_command},
    {"error", "err", error_command},
    {0, 0, 0}
};

int main() {
    cli_loop();

    return 0;
}

int help_command(int argc, const char **argv) {
    for (int i = 0; i < argc; i++) {
        cli_printf("argv[%d]: \"%s\"\n", i, argv[i]);
        cli_printf("bytes: ");
        const unsigned char *p = (unsigned char *)argv[i];
        int j = 0;
        while (1) {
            cli_printf("0x%02x ", p[j]);
            if (p[j] == '\0') break;
            j++;
        }
        cli_printf("\n");
    }

    return 0;
}

int error_command(int argc, const char **argv) {
    if (argc == 1) {
        return CLI_ETEST_ONE;
    }
    if (argc == 2) {
        return CLI_ETEST_TWO;
    }
    return CLI_ETEST_NOT_MAPPED;
}