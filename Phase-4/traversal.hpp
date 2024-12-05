#pragma once
// #include "symbolTable.hpp"
#include "ast.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

void traversal(Start *root);
void checkHeader(vector<Header *> *HeaderList);
void checkProgram(vector<Program *> *programList);
void checkDeclaration(Declaration *declaration, string functionName, string functionScope, vector<FunctionArgumentList *> *argumentList);
void checkFunctionDeclaration(FunctionDeclaration *functionDeclaration);
void checkCompoundStatement(CompoundStatement *compoundStatement, string functionIdentifier, string functionScope, vector<FunctionArgumentList *> *argumentList);
