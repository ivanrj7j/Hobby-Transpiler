#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "tokens.hpp"

using namespace std;

class Tokenizer{

private:
    string fileName;
    vector<Token> tokens;

    string readCode(){
        stringstream content;
        ifstream fileStream;
        fileStream.open(this->fileName);

        if(fileStream.is_open()){
            content << fileStream.rdbuf();
        }
        fileStream.close();
        return content.str();
    }

    void getTokens(){
        string sourceCode = readCode();
        string currentToken;
        char current;

        tokens.clear();

        bool escapeMode = false; // true if any escape character present
        bool commentMode = false; // true if currently parsing a comment
        bool stringMode = false; // true if currently parsing a string
        bool charMode = false; // true if currently parsing a character
        
        for(int i = 0; i < sourceCode.length(); i++){
            char current = sourceCode.at(i);

            if(!escapeMode && current == '\\'){
                escapeMode = true; // TODO: this could lead to potential errors if there is a wild \ character
                continue;
            }

            if(current == '#' && !(stringMode || charMode || commentMode)){
                currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                commentMode = true;
                continue;
            } // if # is found and we are not parsing a string, then the current token is a comment, so comment mode is turned on.

            if(current == '"' && !(commentMode || charMode || escapeMode)){
                if(stringMode){
                    stringMode = false;
                    tokens.push_back(Token(LiteralType{_stringLit}, currentToken));
                    currentToken.clear();
                    // if stringmode is on and " is found, it means the string is ending
                }else{
                    currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                    stringMode = true;
                    // starting to parse a string 
                    continue;
                }
            } // if " is found and we are not parsing a comment, then the current token is a string, so string mode is turned on. there are two possibilities, either the string is starting or ending, both cases are handled inside the code.

            if(current == '\'' && !(commentMode || stringMode || escapeMode)){
                if(charMode){
                    charMode = false;
                    if(currentToken.length() != 1){
                        cerr << "The length of a character should exactly be 1.";
                        throw invalid_argument("The length of the character should exactly be 1.");
                    }// if the given character token does not have one character, then it is invalid.
                    tokens.push_back(Token(LiteralType{_charLit}, currentToken));
                    currentToken.clear();
                }else{
                    currentToken.clear(); // TODO: have better error handling, this will cause problems in future, here just for initial testing purposes
                    charMode = true;
                    continue;
                }
            }//same logic used for string parsing

            if(current == '\n'){
                if(commentMode){
                    commentMode = false;
                    tokens.push_back(Token(currentToken));
                    currentToken.clear();
                }
                tokens.push_back(Token(WhiteSpaceType{_newLine}));
            }// terminating a comment

            if(!(commentMode || charMode || stringMode) && (current == '\t' || current == ' ')){
                WhiteSpaceType type = _space;
                if(current == '\t') type = _tab;
                tokens.push_back(Token(type));
                continue;
            }// pushing in whitespaces

            currentToken.push_back(current);
            escapeMode = false;
        }
    }

public:
    Tokenizer(string fileName){
        this->fileName = fileName;
        cout << "Initialized Tokenizer" << endl;
        this->getTokens();
        cout << "Tokens generated:" << endl;
        printTokens();
    }
    
    void printTokens(){
        for(Token token: tokens){
            _Token t = token.get();
            switch (t.tokenType)
            {
            case _comment:
                cout << "\nComment: " << t.value << endl;
                break;
            case _literal:
                cout << "\nLiteral: " << t.value << endl;
                break;
            case _whitespace:
                cout << "Whitespace Type enum: " << t.token << endl;
                break;
            }
        }
    }

    ~Tokenizer(){
        cout << "Exiting...";
    }
};