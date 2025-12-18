/**
 * @file main.cpp
 * @brief Entry point for the HoPiler transpiler
 * 
 * This file contains the main() function which orchestrates the entire transpilation pipeline:
 * 1. Validates command-line arguments
 * 2. Creates and runs the Tokenizer (lexical analysis)
 * 3. Creates and runs the Parser (syntax analysis)
 * 
 * The transpiler expects the source filename as a command-line argument.
 * 
 * Usage: HoPiler <source_file>
 * Example: HoPiler program.ho
 * 
 * @author HoPiler Project
 */

#include <iostream>
#include "tokenizer.hpp"
#include "parser.hpp"

using namespace std;

/**
 * @brief Main entry point of the HoPiler transpiler
 * 
 * @param argc Argument count (number of command-line arguments)
 * @param argv Argument vector (array of command-line argument strings)
 * 
 * @return EXIT_SUCCESS (0) if transpilation completes
 *         EXIT_FAILURE (1) if argument validation fails
 */
int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "HoPiler failed. No source coude given! When running the code, also include the filename like:" << endl
             << "HoPiler fileName.ho";
        return EXIT_FAILURE;
    }

    string fileName = string(argv[1]);
    Tokenizer tokenizer(fileName);
    Parser parser(tokenizer.getTokens());

    return EXIT_SUCCESS;
}