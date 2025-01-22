# C Language Lexical and Syntactical Parser

This project is a lexical and syntactical parser for the C language. It uses a custom grammar based on YACC and a stack system of tokens for analysis.


### Files

- **defs.c**: Contains common definitions used by the lexer.
- **gramatica.txt**: Contains the custom grammar rules.
- **input.c**: Example input file for the parser.
- **lex.c**: Implements the lexical analysis.
- **pilha.c**: Implements the stack operations for tokens.
- **sint.c**: Implements the syntactical analysis.
- **tabsimb.c**: Implements the symbol table.
- **tk.h**: Contains the token definitions.
- **token.h**: Defines the `Token` structure.

## Getting Started

### Prerequisites

- GCC (GNU Compiler Collection)
- VSCode (optional, for development)

### Building the Project

To build the project, you can use the provided tasks in VSCode or run the following command:

```sh
gcc -fdiagnostics-color=always -g sint.c lex.c pilha.c defs.c tabsimb.c -o sint
```

### Running the parser
To run the parser, use the following command with the input file as an argument, as such:
```sh
./sint input.c
```

### Debugging
You can use the provided `launch.json` configuration to debug the project in VSCode.

### Usage
The parser reads an input C file and performs lexical and syntactical analysis based on the custom grammar defined in grammar.txt. The recognized tokens are written to an output txt file, and the result is printed on the console (pass/fail).

### Custom Grammar
The custom grammar is defined in the `gramatica.txt` and is based on the the YACC format. It includes rules for declarations, functions, expressions, and control structures.

### License
This project is licensed under the GPLv3 License - see the LICENSE file for details.
