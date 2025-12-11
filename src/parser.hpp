#include "expNode.hpp"
#include "tokens.hpp"
#include <iostream>
#include <vector>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    ExpressionNode head;

    void parseTree()
    {
        cout << "Tree is being parsed" << endl;
    }

    void _printTree(ExpressionNode node)
    {
        for(ExpressionNode n:node.getChildren())
        {
            _printTree(n);
        }
        node.print();
    }

public:
    Parser(vector<Token> tokens)
        : tokens(tokens)
        , head(Token())
    {
        cout << "Received " << tokens.size() << " tokens." << endl;
        cout << "Parsing tree" << endl;
        parseTree();
        cout << "Tree parsed" << endl;
        printTree();
    }

    ExpressionNode getTree()
    {
        return head;
    }

    void printTree()
    {
        cout << "\n\n";
        _printTree(head);
        cout << "\n\n";
    }
};
