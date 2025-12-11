#include "tokens.hpp"
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
};