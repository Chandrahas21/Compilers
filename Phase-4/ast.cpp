#include "ast.hpp"

Node::Node() : row(0), column(0) {}

Start::Start(vector<Header *> *headerList, vector<Program *> *programList, int row, int column) {
    this->headerList = headerList ? headerList : new vector<Header *>();
    this->programList = programList ? programList : new vector<Program *>();
    this->row = row;
    this->column = column;
}

Header::Header(int isHeader, char *header, int row, int column) {
    this->isHeader = isHeader;
    this->header = header;
    this->macroIdentifier = nullptr;
    this->constantValue = 0.0f;
    this->row = row;
    this->column = column;
}

Header::Header(int isHeader, char *macroIdentifier, float constantValue, int row, int column) {
    this->isHeader = isHeader;
    this->header = nullptr;
    this->macroIdentifier = macroIdentifier;
    this->constantValue = constantValue;
    this->row = row;
    this->column = column;
}

Program::Program(int isFunction, Declaration *declaration, int row, int column) {
    this->isFunction = isFunction;
    this->declaration = declaration;
    this->functionDeclaration = nullptr;
    this->row = row;
    this->column = column;
}

Program::Program(int isFunction, FunctionDeclaration *functionDeclaration, int row, int column) {
    this->isFunction = isFunction;
    this->declaration = nullptr;
    this->functionDeclaration = functionDeclaration;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, AssignmentExpression *assignmentExpression, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = assignmentExpression;
    this->declaration = nullptr;
    this->inOut = nullptr;
    this->conditionalStatement = nullptr;
    this->jumpStatement = nullptr;
    this->compoundStatement = nullptr;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, Declaration *declaration, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = declaration;
    this->inOut = nullptr;
    this->conditionalStatement = nullptr;
    this->jumpStatement = nullptr;
    this->compoundStatement = nullptr;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, InOut *inOut, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = nullptr;
    this->inOut = inOut;
    this->conditionalStatement = nullptr;
    this->jumpStatement = nullptr;
    this->compoundStatement = nullptr;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, ConditionalStatement *conditionalStatement, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = nullptr;
    this->inOut = nullptr;
    this->conditionalStatement = conditionalStatement;
    this->jumpStatement = nullptr;
    this->compoundStatement = nullptr;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, JumpStatement *jumpStatement, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = nullptr;
    this->inOut = nullptr;
    this->conditionalStatement = nullptr;
    this->jumpStatement = jumpStatement;
    this->compoundStatement = nullptr;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, CompoundStatement *compoundStatement, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = nullptr;
    this->inOut = nullptr;
    this->conditionalStatement = nullptr;
    this->jumpStatement = nullptr;
    this->compoundStatement = compoundStatement;
    this->iterativeStatement = nullptr;
    this->row = row;
    this->column = column;
}

Statement::Statement(int flagStatement, IterativeStatement *iterativeStatement, int row, int column) {
    this->flagStatement = flagStatement;
    this->assignmentExpression = nullptr;
    this->declaration = nullptr;
    this->inOut = nullptr;
    this->conditionalStatement = nullptr;
    this->jumpStatement = nullptr;
    this->compoundStatement = nullptr;
    this->iterativeStatement = iterativeStatement;
    this->row = row;
    this->column = column;
}

InOut::InOut() {
}

InOut::InOut(int isWrite, vector<Print *> *printList, int row, int column) {
    this->isWrite = isWrite;
    this->scanList = new vector<Scan *>();
    this->printList = printList ? printList : new vector<Print *>();
    this->row = row;
    this->column = column;
}

InOut::InOut(int isWrite, vector<Scan *> *scanList, int row, int column) {
    this->isWrite = isWrite;
    this->scanList = scanList ? scanList : new vector<Scan *>();
    this->printList = new vector<Print *>();
    this->row = row;
    this->column = column;
}

Scan::Scan(char *scanIdentifier, string scope, int row, int column) {
    this->isWrite = 0;
    this->scanIdentifier = scanIdentifier;
    this->scanList = new vector<Scan *>();
    this->printList = new vector<Print *>();
    this->scope = scope;
    this->row = row;
    this->column = column;
}

Print::Print(int flagPrint, char *printIdentifier, int row, int column) {
    this->flagPrint = flagPrint;
    this->isWrite = 1;
    this->printIdentifier = printIdentifier;
    this->expression = nullptr;
    this->scanList = new vector<Scan *>();
    this->printList = new vector<Print *>();
    this->row = row;
    this->column = column;
}

Print::Print(int flagPrint, Expression *expression, int row, int column) {
    this->flagPrint = flagPrint;
    this->isWrite = 1;
    this->printIdentifier = nullptr;
    this->expression = expression;
    this->scanList = new vector<Scan *>();
    this->printList = new vector<Print *>();
    this->row = row;
    this->column = column;
}

Expression::Expression() {
}

ConstantValue::ConstantValue(int flagConstant, float fval, int row, int column) {
    this->flagConstant = flagConstant;
    this->fval = fval;
    this->bval = false;
    this->sval = nullptr;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(int flagConstant, bool bval, int row, int column) {
    this->flagConstant = flagConstant;
    this->fval = 0.0f;
    this->bval = bval;
    this->sval = nullptr;
    this->row = row;
    this->column = column;
}

ConstantValue::ConstantValue(int flagConstant, char *sval, int row, int column) {
    this->flagConstant = flagConstant;
    this->fval = 0.0f;
    this->bval = false;
    this->sval = sval;
    this->row = row;
    this->column = column;
}

FunctionCall::FunctionCall(int flagFunctionCall, char *functionCallIdentifier, string scope, int row, int column) {
    this->flagFunctionCall = flagFunctionCall;
    this->functionCallIdentifier = functionCallIdentifier;
    this->argumentList = new vector<Expression *>();
    this->scope = scope;
    this->row = row;
    this->column = column;
}

FunctionCall::FunctionCall(int flagFunctionCall, char *functionCallIdentifier, vector<Expression *> *argumentList, string scope, int row, int column) {
    this->flagFunctionCall = flagFunctionCall;
    this->functionCallIdentifier = functionCallIdentifier;
    this->argumentList = argumentList ? argumentList : new vector<Expression *>();
    this->scope = scope;
    this->row = row;
    this->column = column;
}

BasicExpression::BasicExpression(int flagBasic, char *basicIdentifier, string scope, int row, int column) {
    this->flagBasic = flagBasic;
    this->basicIdentifier = basicIdentifier;
    this->constantValue = nullptr;
    this->expression = nullptr;
    this->scope = scope;
    this->row = row;
    this->column = column;
}

BasicExpression::BasicExpression(int flagBasic, ConstantValue *constantValue, int row, int column) {
    this->flagBasic = flagBasic;
    this->basicIdentifier = nullptr;
    this->constantValue = constantValue;
    this->expression = nullptr;
    this->row = row;
    this->column = column;
}

BasicExpression::BasicExpression(int flagBasic, Expression *expression, int row, int column) {
    this->flagBasic = flagBasic;
    this->basicIdentifier = nullptr;
    this->constantValue = nullptr;
    this->expression = expression;
    this->row = row;
    this->column = column;
}

PostfixExpression::PostfixExpression(int flagPostfix, BasicExpression *basicExpression, int row, int column) {
    this->flagPostfix = flagPostfix;
    this->basicExpression = basicExpression;
    this->functionCall = nullptr;
    this->postfixIdentifier = nullptr;
    this->opList = new vector<UnaryOperator>();
    this->expression = nullptr;
    this->row = row;
    this->column = column;
}

PostfixExpression::PostfixExpression(int flagPostfix, FunctionCall *functionCall, int row, int column) {
    this->flagPostfix = flagPostfix;
    this->basicExpression = nullptr;
    this->functionCall = functionCall;
    this->postfixIdentifier = nullptr;
    this->opList = new vector<UnaryOperator>();
    this->expression = nullptr;
    this->row = row;
    this->column = column;
}

PostfixExpression::PostfixExpression(int flagPostfix, char *postfixIdentifier, MemberVariable memberVariable1, string scope, int row, int column) {
    this->flagPostfix = flagPostfix;
    this->basicExpression = nullptr;
    this->functionCall = nullptr;
    this->postfixIdentifier = postfixIdentifier;
    this->memberVariable1 = memberVariable1;
    this->opList = new vector<UnaryOperator>();
    this->expression = nullptr;
    this->scope = scope;
    this->row = row;
    this->column = column;
}

PostfixExpression::PostfixExpression(int flagPostfix, char *postfixIdentifier, MemberVariable memberVariable1, MemberVariable memberVariable2, string scope, int row, int column) {
    this->flagPostfix = flagPostfix;
    this->basicExpression = nullptr;
    this->functionCall = nullptr;
    this->postfixIdentifier = postfixIdentifier;
    this->memberVariable1 = memberVariable1;
    this->memberVariable2 = memberVariable2;
    this->opList = new vector<UnaryOperator>();
    this->expression = nullptr;
    this->scope = scope;
    this->row = row;
    this->column = column;
}

PostfixExpression::PostfixExpression(int flagPostfix, char *postfixIdentifier, Expression *expression, string scope, int row, int column) {
    this->flagPostfix = flagPostfix;
    this->basicExpression = nullptr;
    this->functionCall = nullptr;
    this->postfixIdentifier = postfixIdentifier;
    this->expression = expression;
    this->opList = new vector<UnaryOperator>();
    this->scope = scope;
    this->row = row;
    this->column = column;
}

UnaryExpression::UnaryExpression(PostfixExpression *postfixExpression, int row, int column) {
    this->postfixExpression = postfixExpression;
    this->opList = new vector<UnaryOperator>();
    this->row = row;
    this->column = column;
}

AssignmentExpression::AssignmentExpression(int isAssignment, PostfixExpression *postfixExpression, Expression *expression, int row, int column) {
    this->isAssignment = isAssignment;
    this->postfixExpression = postfixExpression;
    this->expression = expression;
    this->row = row;
    this->column = column;
}

AssignmentExpression::AssignmentExpression(int isAssignment, Expression *expression, int row, int column) {
    this->isAssignment = isAssignment;
    this->postfixExpression = nullptr;
    this->expression = expression;
    this->row = row;
    this->column = column;
}

BinaryExpression::BinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, int row, int column) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
    this->row = row;
    this->column = column;
}

Declaration::Declaration(int isVector, char *declarationType, char *isInFunction, vector<DeclarationIndex *> *declarationList, int row, int column) {
    this->isVector = isVector;
    this->declarationType = declarationType;
    this->isInFunction = isInFunction;
    this->declarationList = declarationList ? declarationList : new vector<DeclarationIndex *>();
    this->row = row;
    this->column = column;
}

DeclarationIndex::DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, int row, int column, string scope) {
    this->flagDeclarationIndex = flagDeclarationIndex;
    this->declarationIdentifier = declarationIdentifier;
    this->expression = nullptr;
    this->expressionList = new vector<Expression *>();
    this->row = row;
    this->column = column;
    this->scope = scope;
}

DeclarationIndex::DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, Expression *expression, int row, int column, string scope) {
    this->flagDeclarationIndex = flagDeclarationIndex;
    this->declarationIdentifier = declarationIdentifier;
    this->expression = expression;
    this->expressionList = new vector<Expression *>();
    this->row = row;
    this->column = column;
    this->scope = scope;
}

DeclarationIndex::DeclarationIndex(int flagDeclarationIndex, char *declarationIdentifier, vector<Expression *> *expressionList, int row, int column, string scope) {
    this->flagDeclarationIndex = flagDeclarationIndex;
    this->declarationIdentifier = declarationIdentifier;
    this->expression = nullptr;
    this->expressionList = expressionList ? expressionList : new vector<Expression *>();
    this->row = row;
    this->column = column;
    this->scope = scope;
}

FunctionDeclaration::FunctionDeclaration(int isDefaultFunction, char *returnType, char *functonIdentifier, CompoundStatement *compoundStatement, int row, int column) {
    this->isDefaultFunction = isDefaultFunction;
    this->returnType = returnType;
    this->functonIdentifier = functonIdentifier;
    this->argumentList = new vector<FunctionArgumentList *>();
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

FunctionDeclaration::FunctionDeclaration(int isDefaultFunction, char *returnType, char *functonIdentifier, vector<FunctionArgumentList *> *argumentList, CompoundStatement *compoundStatement, int row, int column) {
    this->isDefaultFunction = isDefaultFunction;
    this->returnType = returnType;
    this->functonIdentifier = functonIdentifier;
    this->argumentList = argumentList ? argumentList : new vector<FunctionArgumentList *>();
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

FunctionArgumentList::FunctionArgumentList(char *ArgType, char *ArgIdentifier, int row, int column) {
    this->ArgType = ArgType;
    this->ArgIdentifier = ArgIdentifier;
    this->row = row;
    this->column = column;
}

CompoundStatement::CompoundStatement(vector<Statement *> *statementList, int row, int column) {
    this->statementList = statementList ? statementList : new vector<Statement *>();
    this->row = row;
    this->column = column;
}

ConditionalStatement::ConditionalStatement(int flagSelective, Expression *expression, CompoundStatement *compoundStatement1, vector<ElseIf *> *elseIfList, int row, int column) {
    this->flagSelective = flagSelective;
    this->expression = expression;
    this->compoundStatement1 = compoundStatement1;
    this->elseIfList = elseIfList ? elseIfList : new vector<ElseIf *>();
    this->compoundStatement2 = nullptr;
    this->row = row;
    this->column = column;
}

ConditionalStatement::ConditionalStatement(int flagSelective, Expression *expression, CompoundStatement *compoundStatement1, vector<ElseIf *> *elseIfList, CompoundStatement *compoundStatement2, int row, int column) {
    this->flagSelective = flagSelective;
    this->expression = expression;
    this->compoundStatement1 = compoundStatement1;
    this->elseIfList = elseIfList ? elseIfList : new vector<ElseIf *>();
    this->compoundStatement2 = compoundStatement2;
    this->row = row;
    this->column = column;
}

ElseIf::ElseIf(Expression *expression, CompoundStatement *compoundStatement, int row, int column) {
    this->expression = expression;
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

IterativeStatement::IterativeStatement(int isWhile, Expression *expression2, CompoundStatement *compoundStatement, int row, int column) {
    this->isWhile = isWhile;
    this->assignmentexpression1 = nullptr;
    this->declaration = nullptr;
    this->expression2 = expression2;
    this->assignmentexpression3 = nullptr;
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

IterativeStatement::IterativeStatement(int isWhile, AssignmentExpression *expression1, Expression *expression2, AssignmentExpression *expression3, CompoundStatement *compoundStatement, int row, int column) {
    this->isWhile = isWhile;
    this->assignmentexpression1 = expression1;
    this->declaration = nullptr;
    this->expression2 = expression2;
    this->assignmentexpression3 = expression3;
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

IterativeStatement::IterativeStatement(int isWhile, Declaration *declaration, Expression *expression2, AssignmentExpression *expression3, CompoundStatement *compoundStatement, int row, int column) {
    this->isWhile = isWhile;
    this->assignmentexpression1 = nullptr;
    this->declaration = declaration;
    this->expression2 = expression2;
    this->assignmentexpression3 = expression3;
    this->compoundStatement = compoundStatement;
    this->row = row;
    this->column = column;
}

JumpStatement::JumpStatement(int flagJump, int row, int column) {
    this->flagJump = flagJump;
    this->expression = nullptr;
    this->row = row;
    this->column = column;
}

JumpStatement::JumpStatement(int flagJump, Expression *expression, int row, int column) {
    this->flagJump = flagJump;
    this->expression = expression;
    this->row = row;
    this->column = column;
}