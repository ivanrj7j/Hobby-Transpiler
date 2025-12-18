# Hobby-Transpiler

This is a hobby project exploring how programming languages work by building a transpiler from scratch. The HoPiler transpiler converts HoLang source code (`.ho` files) to C code.

## Overview

The transpiler follows a classic three-phase compilation model:

1. **Tokenization** - Lexical analysis that breaks source code into tokens
2. **Parse Tree Generation** - Syntax analysis that converts tokens into an expression tree
3. **Code Generation** - Converts the parse tree to target code (C)

## Philosophy

This project is a learning exercise following the [Language Implementation playlist](https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs) as a reference, but with a custom architecture and design decisions tailored to this implementation.

## Compilation Pipeline

The transpiler processes HoLang source code through the following stages:

### Tokenization

The first step where the source code is scanned character-by-character and converted into tokens of various types:
- Keywords (if, while, int, etc.)
- Identifiers (variable names, function names)
- Literals (numbers, strings, characters)
- Operators (arithmetic, comparison, logical, assignment)
- Delimiters (parentheses, braces, brackets)
- Comments and whitespace

### Parse Tree Generation

Tokens are converted into an expression tree structure where:
- Each **subtree is an expression tree**
- Each **leaf node is a token**
- Parent nodes represent operations/operators
- Child nodes represent operands

This hierarchical representation makes code generation straightforward.

### Code Generation

The final phase converts the parse tree into compiled/transpiled C code. A well-structured parse tree makes this phase significantly simpler.


## Building and Running

See the CMakeLists.txt for build configuration details.

```bash
cmake --build build
./HoPiler program.ho
```

## Status

Currently supports:
- Variable declaration and assignment statements
- Type checking for assignments
- Basic tokenization and parsing

Future work:
- Extended expression types
- Control flow statements (if, while, for)
- Function definitions
- Code generation to C