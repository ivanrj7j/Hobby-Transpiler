#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Tokenizer{

private:
    string fileName;
    vector<string> tokens;

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

        tokens.clear();
        for(int i = 0; i < sourceCode.length(); i++){
            if(sourceCode.at(i) == ' ' || sourceCode.at(i) == '\t' || sourceCode.at(i) == '\n'){
                this->tokens.push_back(currentToken);
                currentToken.clear();
                continue;
            }

            currentToken.push_back(sourceCode.at(i));
        }
        if(!currentToken.empty()) this->tokens.push_back(currentToken);
    }

public:
    Tokenizer(string fileName){
        this->fileName = fileName;
        cout << "Initialized Tokenizer" << endl;
        this->getTokens();
        cout << "Tokens generated:" << endl;
        for(string token: this->tokens){
            cout << token << endl;
        }
    }
    ~Tokenizer(){
        cout << "Exiting...";
    }
};