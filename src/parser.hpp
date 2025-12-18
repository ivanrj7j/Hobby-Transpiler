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

        for (int i = 0; i < 4; i++) {
            if (keyword == validPairs[i][0] && literal == validPairs[i][1])
                return true;
        }

        return false;
    }

    void parseTree()
    {
        vector<ExpressionNode> operandStack;
        vector<ExpressionNode> expressionStack;

        for (Token token : tokens) {
            _Token t = token.get();

            if (t.tokenType == _comment || (t.tokenType == _whitespace && (t.token == _space || t.token == _tab)))
                continue;

            ExpressionNode node(token);
            node.print();

            // TODO: create a postfix to infix style conversion of expression instead of the current hardcoded assignment only.
            if (t.tokenType == _whitespace) {
                // handling a new line, newline acts as a delimiter like ; in c/c++
                if (operandStack.size() >= 3 && expressionStack.size() >= 1) {
                    ExpressionNode op = expressionStack.at(expressionStack.size() - 1);
                    expressionStack.pop_back();

                    ExpressionNode literal = operandStack.at(operandStack.size() - 1);
                    operandStack.pop_back();

                    ExpressionNode varName = operandStack.at(operandStack.size() - 1);
                    operandStack.pop_back();

                    ExpressionNode dataType = operandStack.at(operandStack.size() - 1);
                    operandStack.pop_back();

                    if (!isValidKeywordLiteralPair(dataType.getToken(), literal.getToken())) {
                        cerr << "Invalid assignment on type " << dataType.getToken() << " with " << literal.getToken() << endl;
                        dataType.print();
                        literal.print();
                        varName.print();
                        cerr << endl;
                        throw invalid_argument("Invalid assignment");
                    }

                    op.addChild(varName);
                    op.addChild(literal);

                    head.addChild(op);
                }
            } else if (t.tokenType == _literal || t.tokenType == _identifier || (t.tokenType == _keyWord && (t.token >= _int && t.token <= _bool))) {
                // handling datatype declarations, identifiers and literals.
                operandStack.push_back(node);
            } else if (t.tokenType == _operator) {
                expressionStack.push_back(node);
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
        cout << "\n\n=====\nParsing tree\n=====\n";
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
