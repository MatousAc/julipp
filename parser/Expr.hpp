#pragma once
#include "../scanner/Token.h"

struct Assign;
struct Binary;
struct Call;
struct Grouping;
struct Literal;
struct Logical;
struct Ternary;
struct Unary;
struct Variable;

struct ExprVisitor {
    virtual void visitAssign(const Assign* expr) = 0;
    virtual void visitBinary(const Binary* expr) = 0;
    virtual void visitCall(const Call* expr) = 0;
    virtual void visitGrouping(const Grouping* expr) = 0;
    virtual void visitLiteral(const Literal* expr) = 0;
    virtual void visitLogical(const Logical* expr) = 0;
    virtual void visitTernary(const Ternary* expr) = 0;
    virtual void visitUnary(const Unary* expr) = 0;
    virtual void visitVariable(const Variable* expr) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor* visitor) = 0;
};

struct Assign : Expr {
    Token name;
    Expr* value;

    Assign(Token name, Expr* value)
        :name{ name }, value{ value } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitAssign(this);
    }
};

struct Binary : Expr {
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right)
        :left{ left }, op{ op }, right{ right } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitBinary(this);
    }
};

struct Call : Expr {
    Expr* callee;
    Token paren;
    vector<Expr*> arguments;

    Call(Expr* callee, Token paren, vector<Expr*> arguments)
        :callee{ callee }, paren{ paren }, arguments{ arguments } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitCall(this);
    }
};

struct Grouping : Expr {
    Expr* expression;

    Grouping(Expr* expression)
        :expression{ expression } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitGrouping(this);
    }
};

struct Literal : Expr {
    LoxType value;

    Literal(LoxType value)
        :value{ value } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitLiteral(this);
    }
};

struct Logical : Expr {
    Expr* left;
    Token op;
    Expr* right;

    Logical(Expr* left, Token op, Expr* right)
        :left{ left }, op{ op }, right{ right } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitLogical(this);
    }
};

struct Ternary : Expr {
    Expr* condition;
    Expr* ifTrue;
    Expr* ifFalse;

    Ternary(Expr* condition, Expr* ifTrue, Expr* ifFalse)
        :condition{ condition }, ifTrue{ ifTrue }, ifFalse{ ifFalse } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitTernary(this);
    }
};

struct Unary : Expr {
    Token op;
    Expr* right;

    Unary(Token op, Expr* right)
        :op{ op }, right{ right } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitUnary(this);
    }
};

struct Variable : Expr {
    Token name;

    Variable(Token name)
        :name{ name } {}

    void accept(ExprVisitor* visitor) override {
        visitor->visitVariable(this);
    }
};

