#pragma once

#include "tokens.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Tokenizer {

private:
    string fileName;
    vector<Token> tokens;

    string readCode()
    {
        stringstream content;
        ifstream fileStream;
        fileStream.open(this->fileName);

        if (fileStream.is_open()) {
            content << fileStream.rdbuf();
        }
        fileStream.close();
        return content.str();
    }

    Token parseCurrentToken(string currentToken)
    {
        if (currentToken == "int")
            return Token(KeyWordType { _int });
        if (currentToken == "char")
            return Token(KeyWordType { _char });
        if (currentToken == "float")
            return Token(KeyWordType { _float });
        if (currentToken == "string")
            return Token(KeyWordType { _string });
        if (currentToken == "bool")
            return Token(KeyWordType { _bool });
        if (currentToken == "if")
            return Token(KeyWordType { _if });
        if (currentToken == "elif")
            return Token(KeyWordType { _elif });
        if (currentToken == "else")
            return Token(KeyWordType { _else });
        if (currentToken == "for")
            return Token(KeyWordType { _for });
        if (currentToken == "while")
            return Token(KeyWordType { _while });
        if (currentToken == "do")
            return Token(KeyWordType { _do });
        if (currentToken == "return")
            return Token(KeyWordType { _return });
        if (currentToken == "break")
            return Token(KeyWordType { _break });
        if (currentToken == "continue")
            return Token(KeyWordType { _continue });
        // keyword tokens

        if (currentToken == "+")
            return Token(OperatorType { _add });
        if (currentToken == "-")
            return Token(OperatorType { _sub });
        if (currentToken == "*")
            return Token(OperatorType { _mul });
        if (currentToken == "/")
            return Token(OperatorType { _div });
        if (currentToken == "%")
            return Token(OperatorType { _mod });
        if (currentToken == "**")
            return Token(OperatorType { _pow });
        // arithmetic operators

        if (currentToken == "and" || currentToken == "&&")
            return Token(OperatorType { _and });
        if (currentToken == "or" || currentToken == "||")
            return Token(OperatorType { _or });
        if (currentToken == "!" || currentToken == "not")
            return Token(OperatorType { _not });
        if (currentToken == "^" || currentToken == "xor")
            return Token(OperatorType { _xor });
        // logical operators

        if (currentToken == "==")
            return Token(OperatorType { _eq });
        if (currentToken == "!=")
            return Token(OperatorType { _neq });
        if (currentToken == ">=")
            return Token(OperatorType { _gte });
        if (currentToken == "<=")
            return Token(OperatorType { _lte });
        if (currentToken == ">")
            return Token(OperatorType { _gt });
        if (currentToken == "<")
            return Token(OperatorType { _lt });
        // comparison operators

        if (currentToken == "=")
            return Token(OperatorType { _ass });
        if (currentToken == "+=")
            return Token(OperatorType { _assAdd });
        if (currentToken == "-=")
            return Token(OperatorType { _assSub });
        if (currentToken == "*=")
            return Token(OperatorType { _assMul });
        if (currentToken == "/=")
            return Token(OperatorType { _assDiv });
        if (currentToken == "%=")
            return Token(OperatorType { _assMod });
        if (currentToken == "**=")
            return Token(OperatorType { _assPow });
        // assignment operators

        if (currentToken == "(")
            return Token(DelimiterType { _bracketOpen });
        if (currentToken == ")")
            return Token(DelimiterType { _bracketClose });
        if (currentToken == "{")
            return Token(DelimiterType { _braceOpen });
        if (currentToken == "}")
            return Token(DelimiterType { _braceClose });
        if (currentToken == "[")
            return Token(DelimiterType { _sqOpen });
        if (currentToken == "]")
            return Token(DelimiterType { _sqClose });
        // delimiters

        if (isdigit(currentToken.at(0))) {
            int totalDecimals = 0;
            for (char x : currentToken) {
                if (!(isdigit(x) || x == '.')) {
                    cerr << "Invalid number(float/int) literal.\n";
                    throw invalid_argument("Invalid number(float/int) literal.");
                }
                if (x == '.')
                    totalDecimals++;
            }
            if (!(totalDecimals == 0 || totalDecimals == 1)) {
                cerr << "Invalid number(float/int) literal. Only one or zero . is permitted\n";
                throw invalid_argument("Invalid number(float/int) literal. Only one or zero . is permitted\n");
            }
            LiteralType type = totalDecimals == 0 ? _intLit : _floatLit;
            return Token(type, currentToken);
        } else if (currentToken.at(0) == '_' || isalpha(currentToken.at(0))) {
            for (char x : currentToken) {
                if (!(x == '_' || isalnum(x))) {
                    cerr << "Identifiers must always start with a _ or an alphabet and contain only _ or alphabet or digits\n";
                    throw invalid_argument("Identifiers must always start with a _ or an alphabet and contain only _ or alphabet or digits\n");
                }
            }
            return Token::identifier(currentToken);
        }

        // return Token(currentToken);
        cerr << "The given token('" << currentToken << "') is invalid";
        throw invalid_argument("The given token('" + currentToken + "') is invalid");
    }

    void _getTokens()
    {
        string sourceCode = readCode();
        string currentToken;
        char current;

        tokens.clear();

        bool escapeMode = false; // true if any escape character present
        bool commentMode = false; // true if currently parsing a comment
        bool stringMode = false; // true if currently parsing a string
        bool charMode = false; // true if currently parsing a character

        for (int i = 0; i < sourceCode.length(); i++) {
            char current = sourceCode.at(i);

            if (!escapeMode && current == '\\') {
                escapeMode = true; // TODO: this could lead to potential errors if there is a wild \ character
                continue;
            }

            if (current == '#' && !(stringMode || charMode || commentMode)) {
                currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                commentMode = true;
                continue;
            } // if # is found and we are not parsing a string, then the current token is a comment, so comment mode is turned on.

            if (current == '"' && !(commentMode || charMode || escapeMode)) {
                if (stringMode) {
                    stringMode = false;
                    tokens.push_back(Token(LiteralType { _stringLit }, currentToken));
                    currentToken.clear();
                    continue;
                    // if stringmode is on and " is found, it means the string is ending
                } else {
                    currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                    stringMode = true;
                    // starting to parse a string
                    continue;
                }
            } // if " is found and we are not parsing a comment, then the current token is a string, so string mode is turned on. there are two possibilities, either the string is starting or ending, both cases are handled inside the code.

            if (current == '\'' && !(commentMode || stringMode || escapeMode)) {
                if (charMode) {
                    charMode = false;
                    if (currentToken.length() != 1 && !(currentToken.length() == 2 && currentToken.at(0) == '\\')) {
                        cerr << "The length of a character should exactly be 1.";
                        throw invalid_argument("The length of the character should exactly be 1.");
                    } // if the given character token does not have one character, then it is invalid.
                    tokens.push_back(Token(LiteralType { _charLit }, currentToken));
                    currentToken.clear();
                    continue;
                } else {
                    currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                    charMode = true;
                    continue;
                }
            } // same logic used for string parsing

            if (current == '\n') {
                if (commentMode) {
                    commentMode = false;
                    tokens.push_back(Token(currentToken));
                } else if (!(commentMode || charMode || stringMode || currentToken.empty())) {
                    try {
                        tokens.push_back(parseCurrentToken(currentToken));
                    } catch (const std::exception& e) {
                        cout << "\n\n=====\nIssue with compiling: " << e.what() << "\n=====\n";
                    }
                }
                tokens.push_back(Token(WhiteSpaceType { _newLine }));
                currentToken.clear();
            } // terminating a comment and any leftover token

            if (!(commentMode || charMode || stringMode) && (current == '\t' || current == ' ' || current == '\n')) {
                WhiteSpaceType type = _tab;
                if (current == ' ') {
                    type = _space;
                    tokens.push_back(parseCurrentToken(currentToken));
                }
                tokens.push_back(Token(type));
                currentToken.clear();
                continue;
            } // pushing in whitespaces
            if (escapeMode) {
                switch (current) {
                case 'n':
                    current = '\n';
                    break;
                case 't':
                    current = '\t';
                    break;
                case 'r':
                    current = '\r';
                    break;
                case 'b':
                    current = '\b';
                    break;
                case 'v':
                    current = '\v';
                    break;
                case 'f':
                    current = '\f';
                    break;
                case '0':
                    current = '\0';
                    break;
                case '\'':
                    break;
                case '"':
                    break;
                case '\\':
                    break;
                default:
                    cerr << "Invalid character after \\ (escape character).";
                    throw invalid_argument("Invalid character after \\ (escape character).");
                }
                escapeMode = false;
            } // formatting escape characters

            currentToken.push_back(current); // pushing the current character to the

            if (!(commentMode || stringMode || charMode)) {
                WhiteSpaceType whitespace;
                switch (current) {
                case '\n':
                    whitespace = _newLine;
                    break;
                case '\t':
                    whitespace = _tab;
                    break;
                case ' ':
                    whitespace = _space;
                    break;
                }
                if (current == '\n' || current == '\t' || current == ' ') {
                    tokens.push_back(Token(whitespace));
                    currentToken.clear();
                }
            } // parsing whitespace tokens
        }

        if (commentMode) {
            commentMode = false;
            tokens.push_back(Token(currentToken));
        } else if (!(commentMode || charMode || stringMode || currentToken.empty())) {
            try {
                tokens.push_back(parseCurrentToken(currentToken));
            } catch (const std::exception& e) {
                cout << "\n\n=====\nIssue with compiling: " << e.what() << "\n=====\n";
            }
        } // pushing the final token
    }

public:
    Tokenizer(string fileName)
    {
        this->fileName = fileName;
        cout << "Initialized Tokenizer" << endl;
        this->_getTokens();
        cout << "Tokens generated:" << endl;
        printTokens();
    }

    vector<Token> getTokens()
    {
        return this->tokens;
    }

    void printTokens()
    {
        for (Token token : tokens) {
            _Token t = token.get();
            switch (t.tokenType) {
            case _comment:
                cout << "\nComment: " << t.value << endl;
                break;
            case _literal:
                cout << "\nLiteral: " << t.value << endl;
                break;
            case _whitespace:
                cout << "Whitespace Type enum: " << t.token << endl;
                break;
            case _keyWord:
                cout << "\nKeyword type: " << t.token << endl;
                break;
            case _operator:
                cout << "Operator type: " << t.token << endl;
                break;
            case _delimiter:
                cout << "Delimiter type: " << t.token << endl;
                break;
            case _identifier:
                cout << "\nIdentifier: " << t.value << endl;
                break;
            }
        }
    }
};