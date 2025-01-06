#include <memory>
#include <vector>
#include <string>
#include <iostream>

class Token {
public:
    TokenType type;
    std::string value;

    // Default constructor
    Token() : type(TokenType::Unknown), value("") {} // Adjust TokenType as needed

    // Parameterized constructor
    Token(TokenType type, const std::string& value) : type(type), value(value) {}
};

enum class TokenType {
    Unknown,
    End,
    If,
    Then,
    Else,
    Identifier,
    Defination,
    Number,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Semicolon,
    LT,
    GT,
    Minus,
    Plus
    // ... other token types ...
};