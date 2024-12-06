#ifndef TRANSPILER_HPP
#define TRANSPILER_HPP

#pragma once
#include "ast.hpp"
#include <fstream>

using namespace std;

void transpiler(Start *start);
string cgStart(Start *start);
string cgHeaderList(vector<Header *> *headerList);
string cgProgramList(vector<Program *> *programList);
string cgDeclaration(Declaration *declaration);
string cgFunctionDeclaration(FunctionDeclaration *functionDeclaration);
string cgCompoundStatement(CompoundStatement *compoundStatement);
string cgInOut(InOut *inOut);
string cgAssignmentExpression(AssignmentExpression *assignmentExpression);
string cgPostFixExpression(PostfixExpression *postfixExpression);
string cgBasicExpression(BasicExpression *basicExpression);
string cgFunctionCall(FunctionCall *functionCall);
string cgConstantValue(ConstantValue *constantValue);
string cgExpression(Expression *expression);
string cgPostFixExpression(PostfixExpression *postfixExpression);
string cgUnaryExpression(UnaryExpression *unaryExpression);
string cgBinaryExpression(BinaryExpression *binaryExpression);
string cgConditionalStatement(ConditionalStatement *conditionalStatement);
string cgIterativeStatement(IterativeStatement *iterativeStatement);
string cgJumpStatement(JumpStatement *jumpStatement);

#endif