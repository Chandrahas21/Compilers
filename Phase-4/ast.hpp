#ifndef AST_HPP
#define AST_HPP

#pragma once
#include "symbolTable.hpp"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

enum UnaryOperator {
    not_op,
    plus_op,
    minus_op,
    inc_op,
    dec_op
};

enum BinaryOperator {
    add_op,
    sub_op,
    mul_op,
    div_op,
    mod_op,
    exp_op,
    and_op,
    or_op,
    equal_op,
    not_equal_op,
    less_op,
    less_equal_op,
    greater_op,
    x3,
    greater_equal_op
};

enum MemberVariable {
    x,
    y,
    slope,
    curve,
    type,
    equation,
    center,
    focus,
    vertex,
    radius,
    a,
    b,
    c,
    f,
    g,
    h,
    delta,
    eccentricity,
    latus_rectum
};

enum DefaultFunctionList {
    toString,
    distance,
    solve,
    sqrt,
    ispoint,
    intersection,
    tangent,
    slopeLine,
    angle,
    type
};

class Node;

class Start;

class Header;
class Program;

class Statement;

class InOut;
class Scan;
class Print;

class Expression;
class ConstantValue;
class FunctionCall;
class BasicExpression;
class PostfixExpression;
class UnaryExpression;
class AssignmentExpression;
class BinaryExpression;

class Declaration;
class DeclarationIndex;

class FunctionDeclaration;
class FunctionArgumentList;

class CompoundStatement;
class ConditionalStatement;
class ElseIf;
class IterativeStatement;
class JumpStatement;

class Node {
public:
    string scope;
    int row, column;
    Node();
    virtual ~Node() = default;
};

class Start : public Node {
public:
    vector<Header *> *headerList;
    vector<Program *> *programList;
    unordered_map<string, GlobalSymTabEntry> *globalSymbolTable;
    Start(vector<Header *> *headerList, vector<Program *> *programList, int row, int column);
    virtual ~Start() = default;
};

class Header : public Node {
public:
    int isHeader; // 1: header, 0: macro
    char *header;
    char *macroIdentifier;
    float constantValue;
    Header(int isHeader, char *header, int row, int column);
    Header(int isHeader, char *macroIdentifier, float constantValue, int row, int column);
    virtual ~Header() = default;
};

class Program : public Node {
public:
    int isFunction; // 0: declaration, 1: functionDeclaration
    Declaration *declaration;
    FunctionDeclaration *functionDeclaration;
    Program(int isFunction, Declaration *declaration, int row, int column);
    Program(int isFunction, FunctionDeclaration *functionDeclaration, int row, int column);
    virtual ~Program() = default;
};

class Statement : public Node {
public:
    int flagStatement; // 0: assignmentExpression, 1: declaration, 2: inOut, 3: conditionalStatement, 4: conditionalStatement 5: iterativeStatement, 6: jumpStatement
    AssignmentExpression *assignmentExpression;
    Declaration *declaration;
    InOut *inOut;
    ConditionalStatement *conditionalStatement;
    JumpStatement *jumpStatement;
    CompoundStatement *compoundStatement;
    IterativeStatement *iterativeStatement;
    Statement(int flagStatement, AssignmentExpression *assignmentExpression, int row, int column);
    Statement(int flagStatement, Declaration *declaration, int row, int column);
    Statement(int flagStatement, InOut *inOut, int row, int column);
    Statement(int flagStatement, ConditionalStatement *conditionalStatement, int row, int column);
    Statement(int flagStatement, JumpStatement *jumpStatement, int row, int column);
    Statement(int flagStatement, CompoundStatement *compoundStatement, int row, int column);
    Statement(int flagStatement, IterativeStatement *iterativeStatement, int row, int column);
    virtual ~Statement() = default;
};

class InOut : public Node {
public:
    int isWrite; // 0: input, 1: output
    vector<Scan *> *scanList;
    vector<Print *> *printList;
    InOut();
    InOut(int isWrite, vector<Print *> *printList, int row, int column);
    InOut(int isWrite, vector<Scan *> *scanList, int row, int column);
    virtual ~InOut() = default;
};

class Scan : public InOut {
public:
    char *scanIdentifier;
    Scan(char *scanIdentifier, string scope, int row, int column);
    virtual ~Scan() = default;
};

class Print : public InOut {
public:
    int flagPrint; // 0: printIdentifier, 1: expression
    char *printIdentifier;
    Expression *expression;
    Print(int flagPrint, char *printIdentifier, int row, int column);
    Print(int flagPrint, Expression *expression, int row, int column);
    virtual ~Print() = default;
};

class Expression : public Node {
public:
    int flagExpression; // 0: binary, 1: unary, 2: postfix
    Expression();
    virtual ~Expression() = default;
};

class ConstantValue : public Node {
public:
    int flagConstant; // 0: float, 1: boolean, 3: string
    float fval;
    bool bval;
    char *sval;
    ConstantValue(int flagConstant, float fval, int row, int column);
    ConstantValue(int flagConstant, bool bval, int row, int column);
    ConstantValue(int flagConstant, char *sval, int row, int column);
    virtual ~ConstantValue() = default;
};

class FunctionCall : public Expression {
public:
    int flagFunctionCall; // 0: functionCall, 1: defaultFunction
    char *functionCallIdentifier;
    vector<Expression *> *argumentList;
    FunctionCall(int flagFunctionCall, char *functionCallIdentifier, string scope, int row, int column);
    FunctionCall(int flagFunctionCall, char *functionCallIdentifier, vector<Expression *> *argumentList, string scope, int row, int column);
    virtual ~FunctionCall() = default;
};

class BasicExpression : public Expression {
public:
    int flagBasic; // 0: constantValue, 1: identifier 2 expression
    char *basicIdentifier;
    ConstantValue *constantValue;
    Expression *expression;
    BasicExpression(int flagBasic, char *basicIdentifier, string scope, int row, int column);
    BasicExpression(int flagBasic, ConstantValue *constantValue, int row, int column);
    BasicExpression(int flagBasic, Expression *expression, int row, int column);
    virtual ~BasicExpression() = default;
};

class PostfixExpression : public Expression {
public:
    int flagPostfix; // 0: basic, 1: functionCall, 2: postfix 3: memberVariable
    BasicExpression *basicExpression;
    FunctionCall *functionCall;
    char *postfixIdentifier;
    MemberVariable memberVariable1;
    MemberVariable memberVariable2;
    vector<UnaryOperator> *opList;
    PostfixExpression(int flagPostfix, BasicExpression *basicExpression, int row, int column);
    PostfixExpression(int flagPostfix, FunctionCall *functionCall, int row, int column);
    PostfixExpression(int flagPostfix, char *postfixIdentifier, MemberVariable memberVariable1, int row, int column);
    PostfixExpression(int flagPostfix, char *postfixIdentifier, MemberVariable memberVariable1, MemberVariable memberVariable2, int row, int column);
    virtual ~PostfixExpression() = default;
};

class UnaryExpression : public Expression {
public:
    PostfixExpression *postfixExpression;
    vector<UnaryOperator> *opList;
    UnaryExpression(PostfixExpression *postfixExpression, int row, int column);
    virtual ~UnaryExpression() = default;
};

class AssignmentExpression : public Expression {
public:
    int isAssignment; // 0: postfixExpression, 1: expression
    PostfixExpression *postfixExpression;
    Expression *expression;
    AssignmentExpression(int isAssignment, PostfixExpression *postfixExpression, Expression *expression, int row, int column);
    AssignmentExpression(int isAssignment, Expression *expression, int row, int column);
    virtual ~AssignmentExpression() = default;
};

class BinaryExpression : public Expression {
public:
    Expression *lhs;
    Expression *rhs;
    BinaryOperator op;
    BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column);
    virtual ~BinaryExpression() = default;
};

class Declaration : public Node {
public:
    char *declarationType;
    char *isInFunction;
    vector<DeclarationIndex *> *declarationList;
    Declaration(char *declarationType, vector<DeclarationIndex *> *declarationList, char *isInFunction, int row, int column);
    virtual ~Declaration() = default;
};

class DeclarationIndex : public Node {
public:
    int flagDeclarationIndex; // 0: declarationIdentifier, 1: expression, 2: expressionList
    char *declarationIdentifier;
    Expression *expression;
    vector<Expression *> *expressionList;
    DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, int row, int column, string scope);
    DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, Expression *expression, int row, int column, string scope);
    DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, vector<Expression *> *expressionList, int row, int column, string scope);
    virtual ~DeclarationIndex() = default;
};

class FunctionDeclaration : public Node {
public:
    int isDefaultFunction; // 0: functionDeclaration, 1: defaultFunction
    char *returnType;
    char *functonIdentifier;
    vector<FunctionArgumentList *> *argumentList;
    CompoundStatement *compoundStatement;
    FunctionDeclaration(int isDefaultFunction, char *returnType, char *functonIdentifier, CompoundStatement *compoundStatement, int row, int column);
    FunctionDeclaration(int isDefaultFunction, char *returnType, char *functonIdentifier, vector<FunctionArgumentList *> *argumentList, CompoundStatement *compoundStatement, int row, int column);
    virtual ~FunctionDeclaration() = default;
};

class FunctionArgumentList : public Node {
public:
    char *ArgType;
    char *ArgIdentifier;
    FunctionArgumentList(char *ArgType, char *ArgIdentifier, int row, int column);
    virtual ~FunctionArgumentList() = default;
};

class CompoundStatement : public Node {
public:
    vector<Statement *> *statementList;
    CompoundStatement(vector<Statement *> *statementList, int row, int column);
    virtual ~CompoundStatement() = default;
};

class ConditionalStatement : public Node {
public:
    int flagSelective; // 0: if, 1: else-if, 2: else
    Expression *expression;
    CompoundStatement *compoundStatement1;
    vector<ElseIf *> *elseIfList;
    CompoundStatement *compoundStatement2;
    ConditionalStatement(int flagSelective, Expression *expression, CompoundStatement *compoundStatement1, vector<ElseIf *> *elseIfList, int row, int column);
    ConditionalStatement(int flagSelective, Expression *expression, CompoundStatement *compoundStatement1, vector<ElseIf *> *elseIfList, CompoundStatement *compoundStatement2, int row, int column);
    virtual ~ConditionalStatement() = default;
};

class ElseIf : public Node {
public:
    Expression *expression;
    CompoundStatement *compoundStatement;
    ElseIf(Expression *expression, CompoundStatement *compoundStatement, int row, int column);
    virtual ~ElseIf() = default;
};

class IterativeStatement : public Node {
public:
    int isWhile; // 0: while, 1: for decl 2: for expr
    AssignmentExpression *assignmentexpression1;
    Declaration *declaration;
    Expression *expression2;
    AssignmentExpression *assignmentexpression3;
    CompoundStatement *compoundStatement;
    IterativeStatement(int isWhile, Expression *expression2, CompoundStatement *compoundStatement, int row, int column);
    IterativeStatement(int isWhile, AssignmentExpression *assignmentexpression1, Expression *expression2, AssignmentExpression *assignmentexpression3, CompoundStatement *compoundStatement, int row, int column);
    IterativeStatement(int isWhile, Declaration *declaration, Expression *expression2, AssignmentExpression *assignmentexpression3, CompoundStatement *compoundStatement, int row, int column);
    virtual ~IterativeStatement() = default;
};

class JumpStatement : public Node {
public:
    int flagJump; // 0: break, 1: continue, 2: return
    Expression *expression;
    JumpStatement(int flagJump, int row, int column);
    JumpStatement(int flagJump, Expression *expression, int row, int column);
    virtual ~JumpStatement() = default;
};

#endif