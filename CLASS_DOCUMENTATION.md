# HoPiler - Complete Class Documentation

This document provides comprehensive documentation of all classes and their methods in the HoPiler transpiler project.

---

## Table of Contents
1. [File Structure and Explanations](#file-structure-and-explanations)
2. [Token Classes](#token-classes)
3. [ExpressionNode Class](#expressionnode-class)
4. [Tokenizer Class](#tokenizer-class)
5. [Parser Class](#parser-class)

---

## File Structure and Explanations

### [src/tokens.hpp](src/tokens.hpp)
**Type:** Header file (token definitions)

**Purpose:** Defines all token-related enumerations and classes that form the foundation of the tokenization system.

**Key Responsibilities:**
- Declares `TokenType`, `KeyWordType`, `LiteralType`, `OperatorType`, `DelimiterType`, and `WhiteSpaceType` enums
- Defines the `_Token` struct for low-level token representation
- Defines the `Token` class as a wrapper around different token types
- Provides operator precedence and associativity information via `getPriority()` and `getAssociativity()` methods
- No implementation file needed (all methods are simple and inline-compatible)

**Dependencies:** Standard library only (`<string>`)

**Size:** ~250 lines

---

### [src/tokenizer.hpp](src/tokenizer.hpp)
**Type:** Header file (lexical analyzer)

**Purpose:** Implements the lexical analysis phase of the transpiler, converting raw source code into a token stream.

**Key Responsibilities:**
- Reads source code files from disk
- Performs character-by-character scanning and recognition
- Handles escape sequences, comments, string/character literals, and whitespace
- Maintains parsing state (comment mode, string mode, etc.)
- Validates tokens (e.g., identifiers must start with `_` or alphabet)
- Produces a vector of `Token` objects for the parser

**Dependencies:** 
- [src/tokens.hpp](src/tokens.hpp)
- Standard library (`<fstream>`, `<sstream>`, `<string>`, `<vector>`, `<iostream>`)

**Key Features:**
- Line comments (starting with `#`)
- String literals (double-quoted)
- Character literals (single-quoted with validation)
- Escape character support (`\n`, `\t`, `\r`, `\b`, `\v`, `\f`, `\0`, `\'`, `\"`, `\\`)
- Comprehensive token type recognition

**Size:** ~400 lines

---

### [src/expNode.hpp](src/expNode.hpp)
**Type:** Header file (AST node definition)

**Purpose:** Defines the `ExpressionNode` class that represents nodes in the abstract syntax tree.

**Key Responsibilities:**
- Encapsulates a token with a tree structure (parent-child relationships)
- Manages child nodes (add, remove, retrieve)
- Provides token accessors (`getTokenType()`, `getToken()`, `getTokenValue()`)
- Prints human-readable token representation

**Dependencies:** 
- [src/tokens.hpp](src/tokens.hpp)
- Standard library (`<iostream>`, `<vector>`)

**Key Features:**
- Tree structure with arbitrary number of children
- Token encapsulation
- Type-safe token access methods
- Formatted output for debugging

**Size:** ~90 lines

---

### [src/parser.hpp](src/parser.hpp)
**Type:** Header file (syntax analyzer)

**Purpose:** Implements the syntax analysis phase of the transpiler, converting tokens into an abstract syntax tree.

**Key Responsibilities:**
- Validates token sequences against language grammar rules
- Type-checks variable assignments
- Builds an Abstract Syntax Tree from tokens
- Handles statement delimiters (newlines)
- Organizes expressions using operator and operand stacks

**Dependencies:** 
- [src/expNode.hpp](src/expNode.hpp)
- [src/tokens.hpp](src/tokens.hpp)
- Standard library (`<iostream>`, `<vector>`)

**Key Features:**
- Two-stack parsing algorithm (operand and expression stacks)
- Type validation for variable assignments
- Tree structure for expressions
- Post-order AST traversal for printing

**Current Limitations:**
- Only supports assignment statements
- TODO: Needs extension to support other expression types and operations
- Rudimentary error reporting

**Size:** ~150 lines

---

### [src/main.cpp](src/main.cpp)
**Type:** Implementation file (entry point)

**Purpose:** The main entry point of the HoPiler transpiler application.

**Key Responsibilities:**
- Command-line argument validation
- Orchestration of the transpilation pipeline
- Error reporting for missing arguments

**Dependencies:** 
- [src/tokenizer.hpp](src/tokenizer.hpp)
- [src/parser.hpp](src/parser.hpp)
- Standard library (`<iostream>`)

**Pipeline:**
1. Validates that exactly one argument (source filename) is provided
2. Creates a `Tokenizer` instance with the filename
3. Creates a `Parser` instance with the tokenizer's output
4. Returns success/failure code

**Error Handling:** Prints diagnostic message if argument count is incorrect

**Size:** ~20 lines

---

### [CMakeLists.txt](CMakeLists.txt)
**Type:** Build configuration file

**Purpose:** Defines the CMake build system configuration for the HoPiler project.

**Key Configuration:**
- Project name: HoPiler
- Language: C++
- Compiler: CMake (generates platform-specific build files like Makefiles)
- Source files: [src/main.cpp](src/main.cpp)

**Size:** Minimal configuration

---

### [README.md](README.md)
**Type:** Documentation file

**Purpose:** Project overview and usage documentation for developers and users.

**Typical Contents:**
- Project description
- Build instructions
- Usage instructions
- Feature list

---

### [LICENSE](LICENSE)
**Type:** Legal file

**Purpose:** Specifies the software license and usage rights.

---

### [programTest/test.ho](programTest/test.ho)
**Type:** Source code file (test program)

**Purpose:** Example program written in the HoPiler language (`*.ho` extension) for testing the transpiler.

**Usage:** Serve as test input to the transpiler to verify correct tokenization and parsing.

---

## Compilation Flow Summary

```
                        ┌─────────────────┐
                        │   main.cpp      │
                        │  (Entry Point)  │
                        └────────┬────────┘
                                 │
                ┌────────────────┴────────────────┐
                │                                 │
                ▼                                 ▼
        ┌──────────────────┐            ┌──────────────────┐
        │   Tokenizer      │            │   tokens.hpp     │
        │  (tokenizer.hpp) │◄───────────│  (Token Classes) │
        └────────┬─────────┘            └──────────────────┘
                 │
        Token Stream (vector<Token>)
                 │
                 ▼
        ┌──────────────────┐            ┌──────────────────┐
        │     Parser       │            │  expNode.hpp     │
        │  (parser.hpp)    │◄───────────│  (AST Nodes)     │
        └────────┬─────────┘            └──────────────────┘
                 │
         AST (Abstract Syntax Tree)
```

---

## Token Classes

### Class: `_Token`
**File:** [src/tokens.hpp](src/tokens.hpp)

A low-level token representation struct that holds the processed token information.

#### Public Members:
- `TokenType tokenType` - The general type of the token (keyword, identifier, literal, etc.)
- `int token` - An enum value representing the specific token type within its category
- `string value` - The string value of the token (used for literals, identifiers, comments)

#### Constructor:
- `_Token(TokenType tokenType, int token, string value)`
  - Initializes a token with the given token type, token enum value, and string value

---

### Class: `Token`
**File:** [src/tokens.hpp](src/tokens.hpp)

A wrapper class that manages different token types and provides high-level token operations.

#### Public Enums:
- `Associativity` - Defines operator associativity
  - `LeftAssoc` - Left-associative operators
  - `RightAssoc` - Right-associative operators
  - `NonAssoc` - Non-associative tokens

#### Constructors:
- `Token()` 
  - Default constructor; creates an expression token
  
- `Token(KeyWordType keywordType)`
  - Creates a keyword token with the specified keyword type
  
- `Token(LiteralType literalType, string value)`
  - Creates a literal token (int, float, string, or char) with the given value
  
- `Token(OperatorType operatorType)`
  - Creates an operator token with the specified operator type
  
- `Token(DelimiterType delimiterType)`
  - Creates a delimiter token (brackets, braces, etc.)
  
- `Token(WhiteSpaceType whiteSpaceType)`
  - Creates a whitespace token (space, tab, newline)
  
- `Token(string comment)`
  - Creates a comment token with the given comment string

#### Public Methods:

- `_Token get()`
  - **Returns:** A `_Token` struct containing the processed token information
  - **Purpose:** Converts the internal Token representation to a `_Token` struct for external use
  - **Throws:** `invalid_argument` if the token type is invalid

- `int getPriority() const`
  - **Returns:** An integer representing operator precedence (0 for non-operators)
  - **Purpose:** Returns the operator precedence/priority for expression parsing
  - **Precedence values:**
    - 80: Power (`**`)
    - 70: NOT (`!`)
    - 60: Multiply, Divide, Modulo (`*`, `/`, `%`)
    - 50: Add, Subtract (`+`, `-`)
    - 40: Greater/Less comparisons (`>`, `<`, `>=`, `<=`)
    - 35: Equality comparisons (`==`, `!=`)
    - 30: Logical AND (`&&`)
    - 25: Logical XOR (`^`)
    - 20: Logical OR (`||`)
    - 10: Assignment operators (`=`, `+=`, `-=`, etc.)

- `Associativity getAssociativity() const`
  - **Returns:** An `Associativity` enum value
  - **Purpose:** Returns the associativity of an operator
  - **Details:**
    - Right-associative: Power, NOT, all assignment operators
    - Left-associative: Most binary operators (arithmetic, comparison, logical)
    - Non-associative: Non-operator tokens

- `static Token identifier(string name)`
  - **Parameters:** `name` - The identifier name
  - **Returns:** A Token object representing an identifier
  - **Purpose:** Factory method to create identifier tokens

---

## ExpressionNode Class

### Class: `ExpressionNode`
**File:** [src/expNode.hpp](src/expNode.hpp)

Represents a node in the abstract syntax tree (AST). Each node can have multiple children and stores a token.

#### Private Members:
- `vector<ExpressionNode> children` - Child nodes in the AST
- `Token token` - The token stored in this node

#### Constructor:
- `ExpressionNode(Token token)`
  - **Parameters:** `token` - The token to store in this node
  - **Purpose:** Initializes an ExpressionNode with the given token

#### Public Methods:

- `void addChild(ExpressionNode node)`
  - **Parameters:** `node` - The child node to add
  - **Purpose:** Appends a child node to this node's children vector

- `void removeChild(int index)`
  - **Parameters:** `index` - The index of the child to remove
  - **Purpose:** Removes the child at the specified index
  - **Note:** Uses vector erase operation

- `vector<ExpressionNode> getChildren()`
  - **Returns:** A copy of the children vector
  - **Purpose:** Provides access to all child nodes

- `void print()`
  - **Purpose:** Prints a human-readable representation of this node's token to stdout
  - **Output format:** Varies by token type:
    - Comments: "Comment: [value]"
    - Literals: "Literal: [value]"
    - Whitespace: "Whitespace: [space/tab/newline]"
    - Keywords: "Keyword type: [enum value]"
    - Operators: "Operator type: [enum value]"
    - Delimiters: "Delimiter type: [enum value]"
    - Identifiers: "Identifier: [name]"
    - Expressions: "Expression"

- `TokenType getTokenType()`
  - **Returns:** The `TokenType` enum of the stored token
  - **Purpose:** Gets the general category type of the token

- `int getToken()`
  - **Returns:** The specific token enum value (e.g., operator type, keyword type)
  - **Purpose:** Gets the specific token type within its category

- `string getTokenValue()`
  - **Returns:** The string value associated with the token
  - **Purpose:** Gets the actual text value (for literals, identifiers, comments)

---

## Tokenizer Class

### Class: `Tokenizer`
**File:** [src/tokenizer.hpp](src/tokenizer.hpp)

Responsible for reading source code and converting it into a stream of tokens.

#### Private Members:
- `string fileName` - Path to the source file to tokenize
- `vector<Token> tokens` - Vector of parsed tokens

#### Private Methods:

- `string readCode()`
  - **Returns:** The entire contents of the source file as a string
  - **Purpose:** Reads the file specified in `fileName` into memory
  - **Details:** Uses `ifstream` and `stringstream` for file I/O

- `Token parseCurrentToken(string currentToken)`
  - **Parameters:** `currentToken` - The token string to parse
  - **Returns:** A `Token` object of the appropriate type
  - **Purpose:** Identifies the token type and creates a corresponding Token object
  - **Token type detection:**
    - Keywords: `int`, `char`, `float`, `string`, `str`, `bool`, `if`, `elif`, `else`, `for`, `while`, `do`, `return`, `break`, `continue`
    - Arithmetic operators: `+`, `-`, `*`, `/`, `%`, `**`
    - Logical operators: `and`, `&&`, `or`, `||`, `!`, `not`, `^`, `xor`
    - Comparison operators: `==`, `!=`, `>=`, `<=`, `>`, `<`
    - Assignment operators: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `**=`
    - Delimiters: `()`, `{}`, `[]`
    - Literals: Numbers (int/float), identifiers starting with `_` or alphabet
  - **Throws:** `invalid_argument` for unrecognized tokens

- `void _getTokens()`
  - **Purpose:** Main tokenization logic; reads source code and generates token stream
  - **Key features:**
    - Handles escape sequences (`\n`, `\t`, `\r`, etc.)
    - Supports line comments (starting with `#`)
    - Handles string literals (double-quoted)
    - Handles character literals (single-quoted)
    - Tracks parsing state with flags: `escapeMode`, `commentMode`, `stringMode`, `charMode`
    - Validates character literals (must be exactly 1 character)
    - Produces whitespace tokens for spaces, tabs, and newlines
  - **Error handling:** Catches exceptions from `parseCurrentToken` and prints error messages without terminating

#### Public Constructor:

- `Tokenizer(string fileName)`
  - **Parameters:** `fileName` - Path to the source code file
  - **Purpose:** Initializes the tokenizer and immediately tokenizes the input file
  - **Side effects:** Prints initialization message and lists all generated tokens
  - **Calls:** `_getTokens()` and `printTokens()`

#### Public Methods:

- `vector<Token> getTokens()`
  - **Returns:** The vector of all parsed tokens
  - **Purpose:** Provides access to the tokenized output

- `void printTokens()`
  - **Purpose:** Prints all tokens to stdout in a human-readable format
  - **Output format:** Different format for each token type:
    - Comments: "Comment: [value]"
    - Literals: "Literal: [value]"
    - Whitespace: "Whitespace Type enum: [enum value]"
    - Keywords: "Keyword type: [enum value]"
    - Operators: "Operator type: [enum value]"
    - Delimiters: "Delimiter type: [enum value]"
    - Identifiers: "Identifier: [value]"

---

## Parser Class

### Class: `Parser`
**File:** [src/parser.hpp](src/parser.hpp)

Responsible for parsing tokens into an abstract syntax tree (AST) and validating syntax.

#### Private Members:
- `vector<Token> tokens` - The token stream from the tokenizer
- `ExpressionNode head` - The root node of the abstract syntax tree

#### Private Methods:

- `bool isValidKeywordLiteralPair(int keyword, int literal)`
  - **Parameters:**
    - `keyword` - A `KeyWordType` enum value representing a data type
    - `literal` - A `LiteralType` enum value representing a literal type
  - **Returns:** `true` if the keyword-literal pair is valid, `false` otherwise
  - **Purpose:** Validates that variable assignments use compatible types
  - **Valid pairs:**
    - `_int` with `_intLit`
    - `_char` with `_charLit`
    - `_string` with `_stringLit`
    - `_float` with `_floatLit`

- `void parseTree()`
  - **Purpose:** Main parsing logic; builds an AST from the token stream
  - **Algorithm:** Uses two stacks:
    - `operandStack` - Stores operands (data types, identifiers, literals)
    - `expressionStack` - Stores operators
  - **Parsing rules:**
    - Comments and whitespace (spaces/tabs) are skipped
    - Data types, identifiers, and literals are pushed to `operandStack`
    - Assignment operators are pushed to `expressionStack`
    - When a newline is encountered (acts as statement delimiter):
      - If conditions are met, the stacks are processed
      - Four operands are popped: data type, variable name, operator, literal
      - Type validation is performed via `isValidKeywordLiteralPair()`
      - An AST node is created with the operator as parent and children as operands
      - Both stacks are cleared
  - **Error handling:** Throws `invalid_argument` on type mismatch
  - **TODO:** Currently handles only assignments; needs extension for other expression types
  - **Side effects:** Prints each token during parsing; prints AST after completion

- `void _printTree(ExpressionNode node)`
  - **Parameters:** `node` - The node to print
  - **Purpose:** Recursively prints the AST in post-order (children first, then parent)
  - **Side effects:** Prints all nodes to stdout

#### Public Constructor:

- `Parser(vector<Token> tokens)`
  - **Parameters:** `tokens` - Token vector from the tokenizer
  - **Purpose:** Initializes the parser and immediately parses the token stream into an AST
  - **Side effects:**
    - Prints initialization message
    - Calls `parseTree()` to build the AST
    - Calls `printTree()` to display the resulting AST
  - **Throws:** `invalid_argument` for type mismatches during parsing

#### Public Methods:

- `ExpressionNode getTree()`
  - **Returns:** The root `ExpressionNode` of the parsed AST
  - **Purpose:** Provides access to the abstract syntax tree

- `void printTree()`
  - **Purpose:** Prints the entire AST to stdout in post-order traversal
  - **Output format:** Calls `_printTree()` on the root node with newlines for formatting

---

## Enum Definitions

All enums are defined in [src/tokens.hpp](src/tokens.hpp):

### TokenType
```
_keyWord         - Keyword tokens
_identifier      - Identifier tokens
_literal         - Literal value tokens
_operator        - Operator tokens
_delimiter       - Delimiter tokens
_comment         - Comment tokens
_whitespace      - Whitespace tokens
_expression      - Expression nodes
```

### KeyWordType
```
_if, _elif, _else, _for, _while, _do, _return, _break, _continue
_int, _float, _string, _char, _bool
```

### LiteralType
```
_intLit      - Integer literal
_floatLit    - Float literal
_stringLit   - String literal
_charLit     - Character literal
```

### OperatorType
```
Arithmetic: _add, _sub, _mul, _div, _mod, _pow
Logical: _and, _or, _not, _xor
Comparison: _eq, _neq, _gte, _lte, _gt, _lt
Assignment: _ass, _assAdd, _assSub, _assMul, _assDiv, _assMod, _assPow
```

### DelimiterType
```
_bracketOpen, _bracketClose     - ()
_braceOpen, _braceClose         - {}
_sqOpen, _sqClose               - []
```

### WhiteSpaceType
```
_space, _tab, _newLine
```

---

## Entry Point

### Function: `main()`
**File:** [src/main.cpp](src/main.cpp)

**Parameters:**
- `argc` - Argument count
- `argv` - Argument vector

**Return:** 
- `EXIT_SUCCESS` (0) on successful execution
- `EXIT_FAILURE` (1) if no source file is provided

**Purpose:** 
- Validates command-line arguments (expects a filename)
- Creates a `Tokenizer` instance with the provided filename
- Creates a `Parser` instance with the tokens from the tokenizer
- Orchestrates the transpilation pipeline

**Error handling:** 
- Prints error message if no source file argument is provided

---

## Compilation Pipeline

```
Source Code File (.ho)
    ↓
Tokenizer::readCode() - reads file
    ↓
Tokenizer::_getTokens() - tokenizes content
    ↓
Token Stream
    ↓
Parser::parseTree() - builds AST
    ↓
Abstract Syntax Tree (AST)
```

---

## Data Flow Summary

1. **Tokenization Phase** (`Tokenizer` class):
   - Reads the source file character by character
   - Recognizes keywords, operators, literals, identifiers, and delimiters
   - Maintains parsing state (comment mode, string mode, etc.)
   - Produces a stream of `Token` objects

2. **Parsing Phase** (`Parser` class):
   - Consumes the token stream
   - Validates syntax rules (e.g., type compatibility)
   - Builds an Abstract Syntax Tree using `ExpressionNode` objects
   - Each node represents either an operator or operand in the expression tree
