## [sh.c](sh.c)

### Features

- Command execution
- Redirection (`<`, `>`, `>>`)
- Pipes (`|`)
- Background execution (`&`)
- Command lists (`;`)

### Code Structure

The shell is structured using a combination of data structures and functions to parse and execute commands.

#### Data Structures

- `struct cmd`: Base structure for commands, which includes `EXEC`, `REDIR`, `PIPE`, `LIST`, and `BACK`.
- `struct execcmd`: Represents an executable command.
- `struct redircmd`: Represents a command with redirection.
- `struct pipecmd`: Represents a command connected by a pipe.
- `struct listcmd`: Represents a list of commands separated by semicolons.
- `struct backcmd`: Represents a command running in the background.

#### Functions

- `runcmd`: Executes the given command.
- `getcmd`: Reads user input.
- `fork1`: Forks a new process and panics on failure.
- `panic`: Prints an error message and exits.
- Constructor functions: `execcmd`, `redircmd`, `pipecmd`, `listcmd`, `backcmd`.
- Parsing functions: `parsecmd`, `parseline`, `parsepipe`, `parseexec`, `parseredirs`, `parseblock`.

### Parsing Logic

The shell uses a series of parsing functions to interpret user input and construct a tree of command structures. Each function handles a specific aspect of shell syntax, such as pipes, redirections, or command lists.

#### Parsing Functions

- `parsecmd`: Parses the entire command line.
- `parseline`: Parses the top-level command structure, handling `&` and `;`.
- `parsepipe`: Parses pipes (`|`) within the command.
- `parseexec`: Parses individual executable commands.
- `parseredirs`: Handles redirections (`<`, `>`, `>>`).
- `parseblock`: Parses command blocks enclosed in parentheses.

#### Tokenization

Commands are tokenized using `gettoken` and `peek` functions to identify keywords, operators, and file names.

#### Execution

The `runcmd` function interprets the command structure and executes the appropriate action based on the command type.

### Limitations

- Supports a maximum of 10 arguments per command (`MAXARGS`).
- Does not handle complex command chaining or advanced shell features like environment variables or job control.