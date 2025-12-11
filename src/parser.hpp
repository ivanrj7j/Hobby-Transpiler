#include "expNode.hpp"
#include "tokens.hpp"
#include <iostream>
#include <vector>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    ExpressionNode head;

    bool isValidKeywordLiteralPair(int keyword, int literal)
    {
        int validPairs[4][2] = {
            { _int, _intLit },
            { _char, _charLit },
            { _string, _stringLit },
            { _float, _floatLit }
        };

        for(int i = 0; i < 4; i++){
            if(keyword == validPairs[i][0] && literal == validPairs[i][1]) return true;
        }

        return false;
    }

    void parseTree()
    {
        vector<ExpressionNode> operandStack;
        vector<ExpressionNode> expressionStack;

        for (Token token : tokens) {
            _Token t = token.get();

            if (t.tokenType == _comment)
                continue;

            ExpressionNode node(token);
            node.print();

            // TODO: create a postfix to infix style conversion of expression instead of the current hardcoded assignment only.
            if (t.tokenType == _operator) {
                expressionStack.push_back(node);
                cout << "Pushed ";
                node.print();
            } else if (t.tokenType == _whitespace) {
                if(expressionStack.empty() || t.token != _newLine) continue;
                ExpressionNode op = expressionStack.at(expressionStack.size() - 1);
                expressionStack.pop_back();
                if (op.getTokenType() == _operator) {
                    if (op.getToken() == _ass) {
                        ExpressionNode c = operandStack.at(operandStack.size() - 1);
                        operandStack.pop_back();

                        ExpressionNode b = operandStack.at(operandStack.size() - 1);
                        operandStack.pop_back();

                        ExpressionNode a = operandStack.at(operandStack.size() - 1);
                        operandStack.pop_back();
                         
                        if (c.getTokenType() == _literal && b.getTokenType() == _identifier && a.getTokenType() == _keyWord) {
                            int typeToken = a.getToken();
                            int valueToken = c.getToken();


                            if(isValidKeywordLiteralPair(typeToken, valueToken)){
                                op.addChild(b);
                                op.addChild(c);
                                head.addChild(op);
                            }
                        }
                    }
                }

            } else {
                operandStack.push_back(node);
                cout << "Pushed operator ";
                node.print();
            }
        }
    }

    void _printTree(ExpressionNode node)
    {
        for (ExpressionNode n : node.getChildren()) {
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
