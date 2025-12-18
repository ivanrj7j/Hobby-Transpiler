/**
 * @file expNode.hpp
 * @brief Defines the ExpressionNode class for AST representation
 * 
 * The ExpressionNode class represents nodes in the abstract syntax tree (AST).
 * Each node encapsulates a token and can have multiple child nodes, forming a
 * hierarchical tree structure for representing program expressions and statements.
 * 
 * @author HoPiler Project
 */

#include "tokens.hpp"
#include <iostream>
#include <vector>
using namespace std;

/**
 * @class ExpressionNode
 * @brief Represents a node in the abstract syntax tree
 * 
 * Each ExpressionNode stores a Token and maintains a list of child nodes.
 * This creates a tree structure where operators can have operands as children,
 * allowing the representation of nested expressions and complex program structures.
 * 
 * Typical usage patterns:
 * - Create a node with a token (operator, operand, etc.)
 * - Add child nodes to represent sub-expressions
 * - Traverse the tree for code generation or optimization
 * 
 * @see Token
 */
class ExpressionNode {
private:
    vector<ExpressionNode> children;
    Token token;

public:
    /**
     * @brief Constructor - creates an ExpressionNode with a given token
     * 
     * @param token The token to store in this node
     * 
     * Initializes the node with a token and an empty children vector.
     * Child nodes can be added later using addChild().
     */
    ExpressionNode(Token token)
    {
        this->token = token;
    }

    /**
     * @brief Adds a child node to this node
     * 
     * @param node The ExpressionNode to add as a child
     * 
     * Appends the given node to the children vector. This is used when building
     * the AST to establish parent-child relationships.
     * 
     * Example: operator_node.addChild(operand_node);
     */
    void addChild(ExpressionNode node)
    {
        this->children.push_back(node);
    }

    /**
     * @brief Removes a child node at the specified index
     * 
     * @param index The zero-based index of the child to remove
     * 
     * Removes the child node at the given index from the children vector.
     * This may be used when restructuring the tree during optimization or transformation.
     * 
     * @note Does not bounds-check; behavior is undefined if index is out of range
     */
    void removeChild(int index)
    {
        this->children.erase(this->children.begin() + index);
    }

    /**
     * @brief Gets all child nodes of this node
     * 
     * @return A copy of the children vector
     * 
     * Returns all child nodes. Modifications to the returned vector do not
     * affect the node's actual children.
     */
    vector<ExpressionNode> getChildren()
    {
        return this->children;
    }

    /**
     * @brief Prints a human-readable representation of this node's token
     * 
     * Outputs information about the token stored in this node:
     * - Comments: "Comment: [value]"
     * - Literals: "Literal: [value]"
     * - Whitespace: "Whitespace: [space/\\t/\\n]"
     * - Keywords: "Keyword type: [enum value]"
     * - Operators: "Operator type: [enum value]"
     * - Delimiters: "Delimiter type: [enum value]"
     * - Identifiers: "Identifier: [name]"
     * - Expressions: "Expression"
     * 
     * This is primarily used for debugging and visualizing the AST.
     * 
     * @see Token::print()
     */
    void print()
    {
        string whitespaces[] = { "space", "\\t", "\\n" };
        _Token t = token.get();
        switch (t.tokenType) {
        case _comment:
            cout << "Comment: " << t.value << endl;
            break;
        case _literal:
            cout << "Literal: " << t.value << endl;
            break;
        case _whitespace:
            cout << "Whitespace: " << whitespaces[t.token] << endl;
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

    /**
     * @brief Gets the token type of the stored token
     * 
     * @return The TokenType enum value (keyword, operator, literal, etc.)
     * 
     * Returns the general category of the token stored in this node.
     */
    TokenType getTokenType()
    {
        return this->token.get().tokenType;
    }

    /**
     * @brief Gets the specific token type within its category
     * 
     * @return The enum value specific to the token's category
     *         (e.g., operator type, keyword type, etc.)
     * 
     * Returns the specific enum value representing the token's subtype.
     * For example, if the token is an operator, returns the OperatorType enum value.
     */
    int getToken()
    {
        return this->token.get().token;
    }

    /**
     * @brief Gets the string value of the stored token
     * 
     * @return The string value (for literals, identifiers, comments)
     * 
     * Returns the actual text representation of the token.
     * Useful for identifiers and literals where the semantic value is the string itself.
     */
    string getTokenValue()
    {
        return this->token.get().value;
    }
};