#include <iostream>
#include "tokenizer.hpp"
#include "parser.hpp"

using namespace std;

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