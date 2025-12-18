/**
 * @file parser.hpp
 * @brief Syntax analyzer for the HoPiler transpiler
 * 
 * The Parser class performs syntax analysis, converting a token stream from the Tokenizer
 * into an Abstract Syntax Tree (AST). It validates syntax rules and builds a hierarchical
 * representation of the program structure.
 * 
 * Current capabilities:
 * - Validates variable declaration and assignment syntax
 * - Type-checks assignments (ensures literal types match declared data types)
 * - Builds AST with proper parent-child relationships
 * - Recognizes statements (delimited by newlines)
 * 
 * Current limitations:
 * - Only supports assignment statements
 * - TODO: Needs extension to handle other expression types, operations, and control structures
 * 
 * This is the second phase of the transpilation pipeline, following tokenization.
 * 
 * @author HoPiler Project
 */

#include "expNode.hpp"
#include "tokens.hpp"
#include <iostream>
#include <vector>

using namespace std;

/**
 * @class Parser
 * @brief Syntax analyzer that builds an Abstract Syntax Tree from tokens
 * 
 * The Parser takes a token stream from the Tokenizer and validates that the tokens
 * follow the grammar rules of the HoPiler language. It then constructs an Abstract
 * Syntax Tree (AST) that represents the hierarchical structure of the program.
 * 
 * Parsing Algorithm:
 * - Uses two stacks: operandStack (for identifiers, literals, data types)
 *                    expressionStack (for operators)
 * - When a newline (statement delimiter) is encountered:
 *   - Validates that we have a complete statement
 *   - Pops elements from stacks in correct order
 *   - Validates type compatibility
 *   - Creates an AST node with the operator as parent and operands as children
 *   - Adds the expression to the AST
 * 
 * Current statement format supported:
 * ```
 * dataType variableName = literal
 * ```
 * 
 * Example: "int x = 5"
 * - "int" is pushed to operandStack (data type keyword)
 * - "x" is pushed to operandStack (identifier)
 * - "=" is pushed to expressionStack (operator)
 * - "5" is pushed to operandStack (literal)
 * - Newline triggers statement processing
 * - Validation: _int with _intLit - OK
 * - AST: [=] with children [x, 5], added to tree
 * 
 * @see ExpressionNode
 * @see Tokenizer
 */
class Parser {
private:
    vector<Token> tokens;
    ExpressionNode head;

    /**
     * @brief Validates that a keyword-literal pair is type-compatible
     * 
     * @param keyword A KeyWordType enum value representing a data type
     * @param literal A LiteralType enum value representing a literal type
     * @return true if the pair is valid (compatible types), false otherwise
     * 
     * Valid pairs (keyword -> literal):
     * - _int -> _intLit
     * - _float -> _floatLit
     * - _string -> _stringLit
     * - _char -> _charLit
     * 
     * This method is used when processing assignment statements to ensure that
     * variables are initialized with literals of the correct type.
     * 
     * Example: isValidKeywordLiteralPair(_int, _intLit) -> true
     *          isValidKeywordLiteralPair(_int, _stringLit) -> false
     */
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



    /**
     * @brief Main parsing loop - converts token stream to AST
     * 
     * This method implements the core parsing algorithm using a two-stack approach:
     * 
     * Stack organization:
     * - operandStack: Stores data type keywords, identifiers, and literals
     * - expressionStack: Stores operators (particularly assignment operators)
     * 
     * Token processing rules:
     * - Skip comments and whitespace (spaces/tabs)
     * - Print each non-whitespace token for debugging
     * - Push identifiers, literals, and data type keywords to operandStack
     * - Push assignment operators to expressionStack
     * - Process complete statements when newline is encountered
     * 
     * Statement processing (triggered on newline):
     * - Checks if stacks contain a complete statement:
     *   - operandStack must have >= 3 elements
     *   - expressionStack must have >= 1 element
     *   - Last expression must be an assignment operator
     * - Pops elements in order:
     *   1. literal (rightmost operand)
     *   2. variableName (identifier)
     *   3. dataType (keyword)
     *   4. operator (assignment)
     * - Validates type compatibility via isValidKeywordLiteralPair()
     * - Creates AST node: operator with children [variableName, literal]
     * - Adds expression to root node's children
     * - Clears both stacks for next statement
     * 
     * Error handling:
     * - Throws invalid_argument if type validation fails
     * - Prints diagnostic information on type mismatch
     * 
     * TODO: Currently only handles assignment expressions.
     *       Needs extension for other operators, expressions, and control structures.
     */
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
                if (operandStack.size() >= 3 && expressionStack.size() >= 1 && expressionStack.at(expressionStack.size()-1).getTokenType() == _operator && expressionStack.at(expressionStack.size()-1).getToken() == _ass) {
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
                    expressionStack.clear();
                    operandStack.clear();
                }
            } else if (t.tokenType == _literal || t.tokenType == _identifier || (t.tokenType == _keyWord && (t.token >= _int && t.token <= _bool))) {
                // handling datatype declarations, identifiers and literals.
                operandStack.push_back(node);
            } else if (t.tokenType == _operator) {
                if(t.token == _ass || t.token == _assAdd || t.token == _assDiv || t.token == _assMod || t.token == _assMul || t.token == _assPow || t.token == _assSub){
                    expressionStack.push_back(node);
                    // for assignment operators 
                }
            }
        }
    }

    /**
     * @brief Recursively prints the AST in post-order traversal
     * 
     * @param node The ExpressionNode to print (usually starts with root)
     * 
     * Performs a post-order tree traversal (children before parent):
     * 1. Recursively prints all children first
     * 2. Then prints the current node
     * 
     * This order ensures that operands are printed before operators,
     * making the output easier to understand in prefix notation.
     * 
     * @see ExpressionNode::print()
     */
    void _printTree(ExpressionNode node)
    {
        for (ExpressionNode n : node.getChildren()) {
            _printTree(n);
        }
        node.print();
    }

public:
    /**
     * @brief Constructor - initializes parser and builds AST from tokens
     * 
     * @param tokens Vector of Token objects from the Tokenizer
     * 
     * Upon construction:
     * 1. Stores the token vector
     * 2. Initializes the root node of the AST
     * 3. Prints diagnostic message showing token count
     * 4. Calls parseTree() to build the AST from tokens
     * 5. Prints diagnostic message on completion
     * 6. Calls printTree() to display the resulting AST
     * 
     * During parsing:
     * - Each token is printed as it's processed
     * - Invalid statements trigger exceptions with diagnostic output
     * - The AST is printed in post-order traversal
     * 
     * Example: Parser parser(tokenizer.getTokens());
     */
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

    /**
     * @brief Gets the root node of the parsed AST
     * 
     * @return The ExpressionNode that is the root of the abstract syntax tree
     * 
     * Returns the root node from which all parsed expressions can be accessed
     * via the tree structure. This is typically used after parsing to access
     * the complete AST for code generation or further processing.
     */
    ExpressionNode getTree()
    {
        return head;
    }

    /**
     * @brief Prints the entire parsed AST in post-order traversal
     * 
     * Formats the output with blank lines before and after the tree for clarity.
     * Calls _printTree() recursively to print the AST starting from the root node.
     * 
     * Output example (for "int x = 5"):
     * ```
     * 
     * 
     * Identifier: x
     * Literal: 5
     * Operator type: [assignment enum value]
     * 
     * 
     * ```
     * 
     * @see _printTree()
     * @see ExpressionNode::print()
     */
    void printTree()
    {
        cout << "\n\n";
        _printTree(head);
        cout << "\n\n";
    }
};
