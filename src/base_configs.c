
#include "cli_loop.h"
#include "cli_config.h"

// Due to limitations from linking, a mapping size needs to 
// be declared in the external file for the CLI to 
// properly know the size
// This is placed AFTER the mapping declaration

// Basic Pattern for a command mapping goes as follows
// 1. Function to be added inside of the command mapping 
//    is declared as "extern." Alternatively, they can be 
//    in a header file
// 2. The function and its associated mapping name is 
//    added to the table as a pair

// Note command_t is a int (*command_t)(int, char**), similar to main
// Example:

/* 
extern int help_command(int, char**);
*/

__attribute__((weak))
const cmd_elem_t command_mapping[] = {
    /*
    {"help", help_command},
    */
};

__attribute__((weak))
const int command_mapping_size = 0; // = sizeof(command_mapping) / sizeof(cmd_elem_t)

__attribute__((weak))
const char *SHELL_PROMPT = "cli default $ ";

void writeln(const char *str) {
    write_str(str, strlen(str)); write_str(NEWLINE, strlen(NEWLINE));
}