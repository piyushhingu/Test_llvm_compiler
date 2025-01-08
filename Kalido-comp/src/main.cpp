#include <iostream>
#include <vector>
#include <fstream>
#include "../include/lexer.h"
#include "../include/parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    printf("hello");
    // Read input from the file specified as an argument
    std::ifstream file(argv[1]); // Assuming argv[1] is the file containing the input
    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Create a lexer instance
    Lexer lexer(input);

    // Tokenize the input
    std::vector<Token> tokens;
    Token token = Token(TokenType::End, ""); // Use an existing member
    while ((token = lexer.getNextToken()).type != TokenType::End) {
        tokens.push_back(token);
    }

    try {
        // Create parser instance
        Parser parser(tokens);
        
        // Parse the expression
        auto ast = parser.parseTopLevel();
        
        std::cout << "Parsing successful! AST structure:" << std::endl;
        ast->print();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 