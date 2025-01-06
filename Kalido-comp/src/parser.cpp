#include "parser.h"
#include <stdexcept>

struct CDFExprAST : public ExprAST {
    std::unique_ptr<ExprAST> value;

    CDFExprAST(std::unique_ptr<ExprAST> value)
        : value(std::move(value)) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "CDF" << std::endl;
        std::cout << std::string(indent + 2, ' ') << "Value:" << std::endl;
        value->print(indent + 4);
    }
};

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), index(0) {}

std::unique_ptr<ExprAST> Parser::parseExpression() {
    // First check for if expression
    if (tokens[index].type == TokenType::If) {
        return parseIfExpr();
    }
    
    // Otherwise parse as binary expression
    auto lhs = parsePrimary();
    return parseBinaryOpRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAST> Parser::parsePrimary() {
    const auto &token = tokens[index];

    switch (token.type) {
        case TokenType::Identifier:
            ++index;
            // Check if it's a function call
            if (index < tokens.size() && tokens[index].type == TokenType::LeftParen) {
                return parseFunctionCall(token.value);
            }
            return std::make_unique<VariableExprAST>(token.value);
            
        case TokenType::Number:
            ++index;
            return std::make_unique<NumberExprAST>(std::stod(token.value));
            
        case TokenType::LeftParen:
            return parseParenExpr();
            
        default:
            throw std::runtime_error("Expected primary expression, got: " + token.value);
    }
}

std::unique_ptr<ExprAST> Parser::parseIfExpr() {
    consume(TokenType::If);
    auto cond = parseExpression();
    consume(TokenType::Then);
    auto thenExpr = parseExpression();
    consume(TokenType::Else);
    auto elseExpr = parseExpression();

    return std::make_unique<IfExprAST>(std::move(cond), std::move(thenExpr), std::move(elseExpr));
}

std::unique_ptr<ExprAST> Parser::parseParenExpr() {
    consume(TokenType::LeftParen);
    auto expr = parseExpression();
    consume(TokenType::RightParen);
    return expr;
}

void Parser::consume(TokenType expected) {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    if (tokens[index].type != expected) {
        throw std::runtime_error("Unexpected token: " + tokens[index].value);
    }
    ++index;
}

std::unique_ptr<ExprAST> Parser::parseBinaryOpRHS(int prec, std::unique_ptr<ExprAST> lhs) {
    while (index < tokens.size()) {
        char op = tokens[index].value[0];
        int tokenPrec = getOperatorPrecedence(op);

        if (tokenPrec < prec) {
            return lhs;
        }

        ++index;
        auto rhs = parsePrimary();

        if (index >= tokens.size()) {
            return std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
        }

        int nextPrec = getOperatorPrecedence(tokens[index].value[0]);
        if (tokenPrec < nextPrec) {
            rhs = parseBinaryOpRHS(tokenPrec + 1, std::move(rhs));
        }

        lhs = std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
    }
    return lhs;
}

int Parser::getOperatorPrecedence(char op) {
    switch (op) {
        case '+': return 10;
        case '-': return 10;
        case '*': return 20;
        case '<': return 5;
        default: return -1;
    }
}

std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
    if (tokens[index].type != TokenType::Identifier)
        throw std::runtime_error("Expected function name in prototype");

    std::string fnName = tokens[index].value;
    ++index;

    if (tokens[index].type != TokenType::LeftParen)
        throw std::runtime_error("Expected '(' in prototype");
    ++index;

    std::vector<std::string> argNames;
    while (tokens[index].type == TokenType::Identifier) {
        argNames.push_back(tokens[index].value);
        ++index;

        if (tokens[index].type == TokenType::Comma) {
            ++index;  // eat ','
        } else if (tokens[index].type == TokenType::RightParen) {
            break;
        }
    }

    if (tokens[index].type != TokenType::RightParen)
        throw std::runtime_error("Expected ')' in prototype");
    ++index;

    return std::make_unique<PrototypeAST>(fnName, std::move(argNames));
}

std::unique_ptr<FunctionAST> Parser::parseDefinition() {
    consume(TokenType::Def);  // eat 'def'
    auto proto = parsePrototype();
    auto body = parseExpression();
    return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
}

std::unique_ptr<PrototypeAST> Parser::parseExtern() {
    consume(TokenType::Extern);  // eat 'extern'
    return parsePrototype();
}

std::unique_ptr<FunctionAST> Parser::parseTopLevelExpr() {
    auto expr = parseExpression();
    // Make an anonymous proto
    auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
}

std::unique_ptr<ExprAST> Parser::parseTopLevel() {
    // Check for function definition
    if (tokens[index].type == TokenType::Def) {
        auto funcAST = parseDefinition();
        return std::move(funcAST);
    }
    
    // Check for if expression
    if (tokens[index].type == TokenType::If) {
        return parseIfExpr();
    }
    
    // Otherwise parse as expression
    return parseExpression();
}

std::unique_ptr<ExprAST> Parser::parseFunctionCall(const std::string& name) {
    ++index;  // eat '('
    std::vector<std::unique_ptr<ExprAST>> args;
    
    if (tokens[index].type != TokenType::RightParen) {
        while (true) {
            args.push_back(parseExpression());
            
            if (tokens[index].type == TokenType::RightParen)
                break;
                
            if (tokens[index].type != TokenType::Comma)
                throw std::runtime_error("Expected ',' or ')' in function call");
            ++index;  // eat ','
        }
    }
    
    ++index;  // eat ')'
    return std::make_unique<CallExprAST>(name, std::move(args));
}
