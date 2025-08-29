
#include "cli_loop.h"
#include "cli_config.h"

// Basic Pattern for a command mapping goes as follows:
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

/*
const cmd_elem_t command_mapping[] = {
    {"help", "h", help_command},
    {0, 0, 0}
};
*/

__attribute__((weak))
const char *SHELL_PROMPT = "cli default $ ";
