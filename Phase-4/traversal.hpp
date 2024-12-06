#ifndef TRAVERSAL_HPP
#define TRAVERSAL_HPP

#pragma once
#include "ast.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void traversal(Start *start);
void checkHeader(vector<Header *> *headerList);
void checkProgram(vector<Program *> *programList);
bool checkDeclarationInArguments(string declarationIdentifier, string dataType, vector<pair<string, string>> *argumentList);
string checkDeclarationInParentScope(string declarationIdentifier, string declarationScope, int checkFlag, GlobalSymTabEntry *functionEntry);
void checkDeclaration(Declaration *declaration, GlobalSymTabEntry *functionEntry);
void checkFunctionDeclaration(FunctionDeclaration *functionDeclaration);
void inOutStatement(InOut *inOut, GlobalSymTabEntry *functionEntry);
void checkAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry);
string curveMemberAccess(MemberVariable memberVariable);
string checkPostfixExpression(PostfixExpression *postfixExpression, GlobalSymTabEntry *functionEntry);
string checkBasicExpression(BasicExpression *basicExpression, GlobalSymTabEntry *functionEntry);
string checkConstantValue(ConstantValue *constantValue);
string checkFunctionCall(FunctionCall *functionCall, GlobalSymTabEntry *functionEntry);
string checkExpression(Expression *expression, GlobalSymTabEntry *functionEntry);
string checkUnaryExpression(UnaryExpression *unaryExpression, GlobalSymTabEntry *functionEntry);
string checkBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry);
void checkCompoundStatement(CompoundStatement *compoundStatement, GlobalSymTabEntry *functionEntry);
void checkConditionalStatement(ConditionalStatement *conditionalStatement, GlobalSymTabEntry *functionEntry);
void checkIterativeStatement(IterativeStatement *iterativeStatement, GlobalSymTabEntry *functionEntry);
void checkJumpStatement(JumpStatement *jumpStatement, GlobalSymTabEntry *functionEntry);

string checkConditionalExpression(Expression *expression, GlobalSymTabEntry *functionEntry);
string checkConditionalBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry);
string checkIterativeAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry);

#endif