#include "lexer.h"
#include <stdexcept>
#include <unordered_map>

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

Token Lexer::getNextToken() {
    // Skip whitespace
    while (position < input.length() && std::isspace(input[position])) {
        position++;
    }

    if (position >= input.length()) {
        return Token(TokenType::End, "");
    }

    // Check for identifiers and keywords
    if (std::isalpha(input[position]) || input[position] == '_') {
        std::string identifier;
        while (position < input.length() && 
               (std::isalnum(input[position]) || input[position] == '_')) {
            identifier += input[position++];
        }

        // Keywords map
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"def", TokenType::Def},
            {"extern", TokenType::Extern},
            {"if", TokenType::If},
            {"then", TokenType::Then},
            {"else", TokenType::Else},
            {"for", TokenType::For},
            {"in", TokenType::In},
            {"var", TokenType::Var}
        };

        auto it = keywords.find(identifier);
        if (it != keywords.end()) {
            return Token(it->second, identifier);
        }
        
        return Token(TokenType::Identifier, identifier);
    }

    // Check for numbers (including decimals)
    if (std::isdigit(input[position]) || input[position] == '.') {
        std::string num;
        bool hasDecimal = false;
        
        while (position < input.length() && 
               (std::isdigit(input[position]) || input[position] == '.')) {
            if (input[position] == '.') {
                if (hasDecimal) break;  // Second decimal point - stop
                hasDecimal = true;
            }
            num += input[position++];
        }
        return Token(TokenType::Number, num);
    }

    // Handle operators and delimiters
    char currentChar = input[position++];
    switch (currentChar) {
        case '(': return Token(TokenType::LeftParen, "(");
        case ')': return Token(TokenType::RightParen, ")");
        case ',': return Token(TokenType::Comma, ",");
        case ';': return Token(TokenType::Semicolon, ";");
        case '+': return Token(TokenType::Plus, "+");
        case '-': return Token(TokenType::Minus, "-");
        case '*': return Token(TokenType::Star, "*");
        case '<': return Token(TokenType::LT, "<");
        case '=': return Token(TokenType::Equals, "=");
        case '!': return Token(TokenType::Not, "!");
    }

    throw std::runtime_error("Unknown character: " + std::string(1, currentChar));
}