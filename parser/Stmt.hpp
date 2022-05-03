#pragma once
#include "Expr.hpp"

struct Block;
struct Break;
struct Continue;
struct Declare;
struct Expression;
struct Function;
struct If;
struct Return;
struct While;

struct StmtVisitor {
    virtual void visitBlock(const Block* stmt) = 0;
    virtual void visitBreak(const Break* stmt) = 0;
    virtual void visitContinue(const Continue* stmt) = 0;
    virtual void visitDeclare(const Declare* stmt) = 0;
    virtual void visitExpression(const Expression* stmt) = 0;
    virtual void visitFunction(const Function* stmt) = 0;
    virtual void visitIf(const If* stmt) = 0;
    virtual void visitReturn(const Return* stmt) = 0;
    virtual void visitWhile(const While* stmt) = 0;
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor* visitor) = 0;
};

struct Block : Stmt {
    vector<Stmt*> statements;

    Block(vector<Stmt*> statements)
        :statements{ statements } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitBlock(this);
    }
};

struct Break : Stmt {
    Break() {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitBreak(this);
    }
};

struct Continue : Stmt {
    Continue() {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitContinue(this);
    }
};

struct Declare : Stmt {
    TokenType scope;
    Token name;
    Expr* initializer;

    Declare(TokenType scope, Token name, Expr* initializer)
        :scope{ scope }, name{ name }, initializer{ initializer } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitDeclare(this);
    }
};

struct Expression : Stmt {
    Expr* expression;

    Expression(Expr* expression)
        :expression{ expression } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitExpression(this);
    }
};

struct Function : Stmt {
    Token name;
    vector<Token> params;
    vector<Stmt*> body;

    Function(Token name, vector<Token> params, vector<Stmt*> body)
        :name{ name }, params{ params }, body{ body } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitFunction(this);
    }
};

struct If : Stmt {
    vector<Expr*> conditions;
    vector<Stmt*> thenBranches;
    Stmt* elseBranch;

    If(vector<Expr*> conditions, vector<Stmt*> thenBranches, Stmt* elseBranch)
        :conditions{ conditions }, thenBranches{ thenBranches }, elseBranch{ elseBranch } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitIf(this);
    }
};

struct Return : Stmt {
    Token keyword;
    Expr* value;

    Return(Token keyword, Expr* value)
        :keyword{ keyword }, value{ value } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitReturn(this);
    }
};

struct While : Stmt {
    Expr* condition;
    Stmt* body;

    While(Expr* condition, Stmt* body)
        :condition{ condition }, body{ body } {}

    void accept(StmtVisitor* visitor) override {
        visitor->visitWhile(this);
    }
};

