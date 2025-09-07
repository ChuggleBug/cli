
Simple command line interface tool for static dispatch tables


## Requirements

### CLI Data Tables[^1]

#### Command Table

In order to execute a command, `const cmd_elem_t command_table[]` needs to be provided. This creates an association between:
  - Command names (and an optional alias)
  - A "main-like" function

It is important to note that arguments that `char**` reference only live as long as the command is running. If the arguments need to persist after the command call, it needs to be copied

### Error Table (Optional)

In order to provide easier error handling. `const cmd_err_elem_t command_err_table[]` can be defined. This is an optional table which defined which maps integer return values to its string representation. After each command, the CLI looks up all non zero return values and ouputs is corresponding error, providing a default if one is not found.

### User I/O

User I/O operations need to be defined for the interface to function properly:
  - A "printf-like" interface
  - One of two input methods
  - A status check to determine wether or not data is present

#### Possible input methods

Two forms of inputs are are allowed and can be toggled with `CLI_TEXT_INPUT_*` in `cli_config.h`. These are:
  - `CLI_TEXT_INPUT_PACKET`: Read an entire data buffer at time. The string buffer is copied into the CLI's own buffer. An example of this can be would be reading an entire TCP packet a time, or any form of data which does not want to handle a string termination sequence. `const char* cli_read_str(void)` would need to be defined
  - `CLI_TEXT_INPUT_PACKET`: Continously read a stream of data. `char cli_read_char(void)` would need to be defined
    - Additionally, `INPUT_STREAM_TERM` should be defined in `cli_config.h`. This is used to denote the string sequence to detect which indicates the end of a "line"

Note that data must always be present at the time of these function calls

#### Data status checking

To be able to detect when data is present `bool cli_read_ready()` needs to be defined. This function can either poll the input source or block until data present (if the application is multithreaded).

#### Ouput options

A single configuration, `NEWLINE`, is provided to denote a newline seqeunce. It is not requred to be used in user applications

#### Additional options

  - `const char *SHELL_PROMPT`: Prompt which appears before each line. A default is provided
  - `CMD_STR_BUFZ`: Static byte buffer size allocated for each line of input
  - `ARGV_MAX`: Maximum number of arguments allowable for each command. Note that a larger `ARGV_MAX` typically needs a larger `CMD_STR_BUFZ`

[^1]: Examples of these tables are provided in `cli_types.h`