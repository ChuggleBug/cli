
#ifndef __CLI_TYPES_H
#define __CLI_TYPES_H

// Name accessable static dispatch table for a command
// line interface. Consists of multiple "main-like" functions

// Underlying strucutre of the table
// type definitions are provided as a convenience, but
// are not required to be used when constructing the table
typedef int (*command_t)(int, const char**);


/*
 * Mapping association between a command name and a "main-like" function.
 * Additionally, a single alias can be defined which can be used
 * to invoke the function instead
*/
typedef struct _cmd_elem_t {
    const char *cmd_name;
    const char *alias;
    command_t command;
} cmd_elem_t;

/*
 * Mapping association between a error number and a string representation of the
 * error.
 * @note In the main CLI loop, err_no 0 will never get called 
 */
typedef struct _cmd_err_elem_t {
    int err_no;
    const char* err_str;
} cmd_err_elem_t;

/*
 * Command table which the CLI loop relies on.
 * To indicate the end of the table, a terminator element
 * that sets at least `cmd_elem_t.cmd_name = 0` is needed. An 
 * example of a table is provided in the variable declaration
*/
extern const cmd_elem_t command_table[];

/*
int example_command(int, char**);

const cmd_elem_t command_table[] = {
    // This command can be called by entering `"example"` or `"ex"`
    {"example", "ex", example_command},
    // Terminator value
    {0, 0, 0}
};
*/


/*
 * Command error table which the CLI loop relies on.
 * To indicate the end of the table, a terminator element
 * that sets BOTH `cmd_err_elem_t.err_no = 0` and
 * `cmd_err_elem_t.err_str = 0`. This table is not required for the
 * CLI loop to function as an empty one is predeclared, but
 * is greatly beneficial. An example of a table is provided 
 * in the variable declaration
*/
extern const cmd_err_elem_t command_err_table[];

/*
// A enum is not required, but helps readability
// Note that in the example table, CLI_ENOT_MAPPED is not included
// When the CLI loop encounters this value, it will recognize an error
// but won't be able to convert it to a string (and provides a default error)
enum CLI_ERRNO {
    // Good idea to set 0 manually so the 
    // complier won't set a real error to 0
    CLI_OK = 0, 
    CLI_EEXAMPLE,
    CLI_ENOT_MAPPED,
};

const cmd_elem_t command_err_table[] = {
    // The CLI loop will print this error everytime a mapped
    // function returns CLI_EEXAMPLE
    {CLI_EEXAMPLE, "Example CLI Error"},
    // Terminator value
    {0, 0}
};
*/

#endif // __CLI_TYPES_H