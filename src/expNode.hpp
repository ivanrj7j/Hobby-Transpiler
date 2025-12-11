#include "tokens.hpp"
#include <iostream>
#include <vector>
using namespace std;

class ExpressionNode {
private:
    vector<ExpressionNode> children;
    Token token;

public:
    ExpressionNode(Token token)
    {
        this->token = token;
    }

    void addChild(ExpressionNode node)
    {
        this->children.push_back(node);
    }

    void removeChild(int index)
    {
        this->children.erase(this->children.begin() + index);
    }

    vector<ExpressionNode> getChildren()
    {
        return this->children;
    }

    void print()
    {
        _Token t = token.get();
        switch (t.tokenType) {
        case _comment:
            cout << "Comment: " << t.value << endl;
            break;
        case _literal:
            cout << "Literal: " << t.value << endl;
            break;
        case _whitespace:
            cout << "Whitespace Type enum: " << t.token << endl;
            break;
        case _keyWord:
            cout << "Keyword type: " << t.token << endl;
            break;
        case _operator:
            cout << "Operator type: " << t.token << endl;
            break;
        case _delimiter:
            cout << "Delimiter type: " << t.token << endl;
            break;
        case _identifier:
            cout << "Identifier: " << t.value << endl;
            break;
        case _expression:
            cout << "Expression" << endl;
            break;
        }
    }
};