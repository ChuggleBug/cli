
#ifndef __CLI_TYPES_H
#define __CLI_TYPES_H

// Name accessable static dispatch table for a command
// line interface. Consists of multiple "main-like" functions

// Underlying strucutre of the table
// type definitions are provided as a convenience, but
// are not required to be used when constructing the table
typedef int (*command_t)(int, const char**);

typedef struct cmd_elem_t {
    const char *cmd_name;
    const char *alias;
    command_t command;
} cmd_elem_t;

extern const cmd_elem_t command_mapping[];
extern const int command_mapping_size;

#endif // __CLI_TYPES_H