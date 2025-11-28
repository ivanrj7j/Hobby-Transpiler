#pragma once

#include <string>
using namespace std;

enum TokenType {_keyWord, _identifier, _literal, _operator, _delimiter, _comment, _whitespace};
enum KeyWordType {_if, _elif, _else, _for, _while, _return, _int, _float, _string, _char};
enum LiteralType {_intLit, _floatLit, _stringLit, _charLit};
enum OperatorType {_add, _sub, _mul, _div, _mod, _and, _or, _not, _eq, _neq};
enum DelimiterType {_bracketOpen, _bracketClose, _braceOpen, _braceClose, _sqOpen, _sqClose};
enum WhiteSpaceType {_space, _tab, _newLine};

class _Token{
    public:
        TokenType tokenType;
        int16_t token;
        string value;

        _Token(TokenType tokenType, int16_t token, string value){
            this->tokenType = tokenType;
            this->token = token;
            this->value = value;
        }
};

class Token{
    private:
        TokenType tokenType;
        KeyWordType keywordType;
        LiteralType literalType;
        OperatorType operatorType;
        DelimiterType delimiterType;
        WhiteSpaceType whiteSpaceType;
        string token;

    public:
        Token(KeyWordType keywordType){
            this->tokenType = _keyWord;
            this->keywordType = keywordType;
        }

        Token(LiteralType literalType, string token){
            this->literalType = literalType;
            this->tokenType = _literal;
            this->token = token;
        }

        Token(OperatorType operatorType){
            this->tokenType = _operator;
            this->operatorType = operatorType;
        }

        Token(DelimiterType delimiterType){
            this->tokenType = _delimiter;
            this->delimiterType = delimiterType;
        }

        Token(WhiteSpaceType whiteSpaceType){
            this->tokenType = _whitespace;
            this->whiteSpaceType = whiteSpaceType;
        }

        Token(string comment){
            this->token = comment;
            this->tokenType = _comment;
        }

        _Token get(){
            int16_t token;
            switch (this->tokenType)            {
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
            default:
                throw invalid_argument("The type of token given is incorrect");
            }
            return _Token(this->tokenType, token, this->token);
        }
};