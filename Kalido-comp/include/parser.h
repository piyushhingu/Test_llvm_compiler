#pragma once

#include <memory>
#include <vector>
#include <string>
#include "lexer.h"
#include <iostream>

// Forward declarations for AST
struct ExprAST {
    virtual ~ExprAST() = default;
    virtual void print(int indent = 0) const = 0;
};

struct NumberExprAST : public ExprAST {
    double value;
    explicit NumberExprAST(double val) : value(val) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Number: " << value << std::endl;
    }
};

struct VariableExprAST : public ExprAST {
    std::string name;
    explicit VariableExprAST(const std::string &name) : name(name) {}
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Variable: " << name << std::endl;
    }
};

struct BinaryExprAST : public ExprAST {
    char op;
    std::unique_ptr<ExprAST> lhs, rhs;

    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Binary op: " << op << std::endl;
        lhs->print(indent + 2);
        rhs->print(indent + 2);
    }
};

struct IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> cond, thenExpr, elseExpr;

    IfExprAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<ExprAST> thenExpr, std::unique_ptr<ExprAST> elseExpr)
        : cond(std::move(cond)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "If" << std::endl;
        std::cout << std::string(indent + 2, ' ') << "Condition:" << std::endl;
        cond->print(indent + 4);
        std::cout << std::string(indent + 2, ' ') << "Then:" << std::endl;
        thenExpr->print(indent + 4);
        std::cout << std::string(indent + 2, ' ') << "Else:" << std::endl;
        elseExpr->print(indent + 4);
    }
};

struct PrototypeAST : public ExprAST {
    std::string name;
    std::vector<std::string> args;

    PrototypeAST(const std::string &name, std::vector<std::string> args)
        : name(name), args(std::move(args)) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Prototype: " << name << std::endl;
        std::cout << std::string(indent + 2, ' ') << "Args: ";
        for (const auto &arg : args) {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
    }
};

struct FunctionAST : public ExprAST {
    std::unique_ptr<PrototypeAST> proto;
    std::unique_ptr<ExprAST> body;

    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : proto(std::move(proto)), body(std::move(body)) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Function definition:" << std::endl;
        proto->print(indent + 2);
        std::cout << std::string(indent + 2, ' ') << "Body:" << std::endl;
        body->print(indent + 4);
    }
};

struct CallExprAST : public ExprAST {
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;

    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
        : callee(callee), args(std::move(args)) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Call: " << callee << std::endl;
        for (const auto &arg : args) {
            arg->print(indent + 2);
        }
    }
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ExprAST> parseExpression();
    std::unique_ptr<ExprAST> parseTopLevel();
    std::unique_ptr<PrototypeAST> parsePrototype();
    std::unique_ptr<FunctionAST> parseDefinition();
    std::unique_ptr<PrototypeAST> parseExtern();
    std::unique_ptr<FunctionAST> parseTopLevelExpr();

private:
    const std::vector<Token> tokens;
    size_t index;

    std::unique_ptr<ExprAST> parsePrimary();
    std::unique_ptr<ExprAST> parseIfExpr();
    std::unique_ptr<ExprAST> parseParenExpr();
    void consume(TokenType expected);
    std::unique_ptr<ExprAST> parseBinaryOpRHS(int prec, std::unique_ptr<ExprAST> lhs);
    int getOperatorPrecedence(char op);
    std::unique_ptr<ExprAST> parseFunctionCall(const std::string& name);
}; 