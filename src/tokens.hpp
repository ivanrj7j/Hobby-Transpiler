/**
 * @file tokens.hpp
 * @brief Defines all token-related enumerations and classes for the HoPiler transpiler
 * 
 * This header file contains:
 * - Token type enumerations (TokenType, KeyWordType, LiteralType, etc.)
 * - _Token: Low-level token representation struct
 * - Token: High-level wrapper class for managing different token types
 * 
 * These classes form the foundation of the tokenization system, allowing the transpiler
 * to represent and manage language tokens with type safety and operator metadata.
 * 
 * @author HoPiler Project
 */

#pragma once

#include <string>
using namespace std;

enum TokenType { _keyWord,
    _identifier,
    _literal,
    _operator,
    _delimiter,
    _comment,
    _whitespace,
    _expression };

/**
 * @enum KeyWordType
 * @brief Enumeration of all supported keywords in the HoPiler language
 * 
 * Control flow: if, elif, else, for, while, do, return, break, continue
 * Data types: int, float, string, char, bool
 */
enum KeyWordType { _if,
    _elif,
    _else,
    _for,
    _while,
    _do,
    _return,
    _break,
    _continue,
    _int,
    _float,
    _string,
    _char,
    _bool };
enum LiteralType { _intLit,
    _floatLit,
    _stringLit,
    _charLit };

/**
 * @enum OperatorType
 * @brief Enumeration of all supported operators in the HoPiler language
 * 
 * Categories:
 * - Arithmetic: add, sub, mul, div, mod, pow
 * - Logical: and, or, not, xor
 * - Comparison: eq, neq, gte, lte, gt, lt
 * - Assignment: ass, assAdd, assSub, assMul, assDiv, assMod, assPow
 */
enum OperatorType { _add,
    _sub,
    _mul,
    _div,
    _mod,
    _pow,
    _and,
    _or,
    _not,
    _xor,
    _eq,
    _neq,
    _gte,
    _lte,
    _gt,
    _lt,
    _ass,
    _assAdd,
    _assSub,
    _assMul,
    _assDiv,
    _assMod,
    _assPow };
enum DelimiterType { _bracketOpen,
    _bracketClose,
    _braceOpen,
    _braceClose,
    _sqOpen,
    _sqClose };

/**
 * @enum WhiteSpaceType
 * @brief Enumeration of whitespace characters recognized by the tokenizer
 */
enum WhiteSpaceType { _space,
    _tab,
    _newLine };

/**
 * @class _Token
 * @brief Low-level token representation struct
 * 
 * This is a simple data structure used to pass token information from the Token class
 * to consumers. It contains the token type, the specific token enum value, and the
 * string representation of the token.
 * 
 * @note This is a helper struct; use the Token class for most operations
 */
class _Token {
public:
    TokenType tokenType;
    int token;
    string value;

    /**
     * @brief Constructs a _Token with the given information
     * 
     * @param tokenType The general category of the token (keyword, operator, etc.)
     * @param token The specific enum value within the category
     * @param value The string representation of the token
     */
    _Token(TokenType tokenType, int token, string value)
    {
        this->tokenType = tokenType;
        this->token = token;
        this->value = value;
    }
};

/**
 * @class Token
 * @brief High-level wrapper class for managing different token types
 * 
 * This class encapsulates different token types (keywords, literals, operators, etc.)
 * and provides methods for:
 * - Type conversion via constructor overloads
 * - Operator precedence and associativity queries
 * - Token information retrieval via the get() method
 * - Identifier token creation via factory method
 * 
 * The Token class uses type-safe constructors to enforce correct token creation.
 * Each token type has its own constructor to ensure type correctness.
 * 
 * @see _Token for the low-level token representation
 */
class Token {
private:
    TokenType tokenType;
    KeyWordType keywordType;
    LiteralType literalType;
    OperatorType operatorType;
    DelimiterType delimiterType;
    WhiteSpaceType whiteSpaceType;
    string token;

public:
    // Associativity for operators
    enum Associativity { LeftAssoc,
        RightAssoc,
        NonAssoc };

    /// @brief Default constructor - creates an expression token
    Token()
    {
        this->tokenType = _expression;
    }

    /// @brief Keyword token constructor
    /// @param keywordType The type of keyword (if, while, int, etc.)
    Token(KeyWordType keywordType)
    {
        this->tokenType = _keyWord;
        this->keywordType = keywordType;
    }

    /**
     * @brief Literal token constructor
     * 
     * @param literalType The type of literal (integer, float, string, or char)
     * @param value The string representation of the literal value
     */
    Token(LiteralType literalType, string value)
    {
        this->literalType = literalType;
        this->tokenType = _literal;
        this->token = value;
    }

    /// @brief Operator token constructor
    /// @param operatorType The type of operator (add, sub, mul, etc.)
    Token(OperatorType operatorType)
    {
        this->tokenType = _operator;
        this->operatorType = operatorType;
    }

    /// @brief Delimiter token constructor
    /// @param delimiterType The type of delimiter (bracket, brace, square bracket)
    Token(DelimiterType delimiterType)
    {
        this->tokenType = _delimiter;
        this->delimiterType = delimiterType;
    }

    /// @brief Whitespace token constructor
    /// @param whiteSpaceType The type of whitespace (space, tab, newline)
    Token(WhiteSpaceType whiteSpaceType)
    {
        this->tokenType = _whitespace;
        this->whiteSpaceType = whiteSpaceType;
    }

    /**
     * @brief Comment token constructor
     * 
     * @param comment The content of the comment (without the # character)
     */
    Token(string comment)
    {
        this->token = comment;
        this->tokenType = _comment;
    }

    /**
     * @brief Converts this Token to a low-level _Token struct
     * 
     * @return _Token struct with token type, enum value, and string representation
     * @throws invalid_argument if the token type is invalid
     * 
     * This method is used to convert the high-level Token representation to a lower-level
     * format suitable for passing to other components or returning from methods.
     */
    _Token get()
    {
        int token;
        switch (this->tokenType) {
        case _keyWord:
            token = this->keywordType;
            break;
        case _literal:
            token = this->literalType;
            break;
        case _operator:
            token = this->operatorType;
            break;
        case _delimiter:
            token = this->delimiterType;
            break;
        case _whitespace:
            token = this->whiteSpaceType;
            break;
        case _comment:
            break;
        case _identifier:
            break;
        case _expression:
            break;
        default:
            throw invalid_argument("The type of token given is incorrect");
        }
        return _Token(this->tokenType, token, this->token);
    }

    /**
     * @brief Gets the operator precedence for this token
     * 
     * @return The precedence value (higher = higher precedence), or 0 for non-operators
     * 
     * Precedence values (from highest to lowest):
     * - 80: Power operator (**)
     * - 70: Logical NOT (!)
     * - 60: Multiply, Divide, Modulo (*, /, %)
     * - 50: Add, Subtract (+, -)
     * - 40: Greater/Less comparisons (>, <, >=, <=)
     * - 35: Equality comparisons (==, !=)
     * - 30: Logical AND (&&)
     * - 25: Logical XOR (^)
     * - 20: Logical OR (||)
     * - 10: Assignment operators (=, +=, -=, etc.)
     * 
     * @note This is typically generated by AI and used for expression parsing
     */
    int getPriority() const
    {
        // this method was generated by ai 
        if (this->tokenType != _operator) {
            return 0;
        }
        switch (this->operatorType) {
        case _pow:
            return 80;
        case _mul:
        case _div:
        case _mod:
            return 60;
        case _add:
        case _sub:
            return 50;
        case _gt:
        case _lt:
        case _gte:
        case _lte:
            return 40;
        case _eq:
        case _neq:
            return 35;
        case _and:
            return 30;
        case _xor:
            return 25;
        case _or:
            return 20;
        case _not:
            return 70;
        case _ass:
        case _assAdd:
        case _assSub:
        case _assMul:
        case _assDiv:
        case _assMod:
        case _assPow:
            return 10;
        default:
            return 0;
        }
    }

    /**
     * @brief Gets the associativity of this operator token
     * 
     * @return The associativity direction (left-to-right, right-to-left, or non-associative)
     * 
     * Associativity rules:
     * - Right-associative: Power, NOT, all assignment operators
     * - Left-associative: Most binary operators (arithmetic, comparison, logical)
     * - Non-associative: Non-operator tokens
     * 
     * This is used during expression parsing to determine evaluation order when multiple
     * operators of the same precedence are present.
     * 
     * @note This method was typically generated by AI
     */
    Associativity getAssociativity() const
    {
        // this method was generated by ai 
        if (this->tokenType != _operator) {
            return NonAssoc;
        }
        switch (this->operatorType) {
        // right-associative operators
        case _pow:
        case _not:
        case _ass:
        case _assAdd:
        case _assSub:
        case _assMul:
        case _assDiv:
        case _assMod:
        case _assPow:
            return RightAssoc;
        // most binary operators are left-associative
        case _mul:
        case _div:
        case _mod:
        case _add:
        case _sub:
        case _gt:
        case _lt:
        case _gte:
        case _lte:
        case _eq:
        case _neq:
        case _and:
        case _xor:
        case _or:
            return LeftAssoc;
        default:
            return NonAssoc;
        }
    }

    /**
     * @brief Factory method to create identifier tokens
     * 
     * @param name The identifier name (variable, function, etc.)
     * @return A Token object representing the identifier
     * 
     * This is a convenience factory method to create identifier tokens, which are
     * different from other token types because they have user-defined names rather
     * than fixed enum values.
     * 
     * Example: Token id = Token::identifier("myVariable");
     */
    static Token identifier(string name)
    {
        Token token;
        token.tokenType = _identifier;
        token.token = name;
        return token;
    }
};