#ifndef __CLI_LOOP_H
#define __CLI_LOOP_H

#include "cli_types.h"

// Main mechanism to dispatch a function from a command table
int exec_from_table(const char* cmd, const cmd_elem_t* table, int argc, const char** argv);

// Main command line entry point
// Consists of a single builtin command
//  - "exit": exits the cli loop
void cli_loop(void);

#endif // __CLI_LOOP_H