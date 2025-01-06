#pragma once

#include <string>

enum class TokenType {
    // Keywords
    Def,
    Extern,
    If,
    Then,
    Else,
    For,
    In,
    Var,
    
    // Identifiers and literals
    Identifier,
    Number,
    
    // Operators
    Plus,    // '+'
    Minus,   // '-'
    Star,    // '*'
    LT,      // '<'
    Equals,  // '='
    Not,     // '!'
    
    // Delimiters
    LeftParen,   // '('
    RightParen,  // ')'
    Comma,       // ','
    Semicolon,   // ';'
    
    // Special
    End,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
    
    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

class Lexer {
public:
    explicit Lexer(const std::string& input);
    Token getNextToken();
    
private:
    std::string input;
    size_t position;
}; 