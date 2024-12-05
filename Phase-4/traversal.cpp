#include "traversal.hpp"
#include "symbolTable.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Start *root;

void traversal(Start *start) {
    root = start;
    checkHeader(start->headerList);
    checkProgram(start->programList);
}

// if a header or macro identifier is included multiple times then what should we do many for loops takes plenty of time
void checkHeader(vector<Header *> *headerList) {
    for (auto headerItem : *headerList) {
        if (headerItem->isHeader == 1) {
            string header = string(headerItem->header);
            if (header.size() >= 2 && (header.substr(header.size() - 2) == ".h\"")) {
                puts("Correct Header.h");
            } else if (header.size() >= 4 && header.substr(header.size() - 5) == ".hpp\"") {
                puts("Correct Header.hpp");
            } else {
                string error = "Invalid header file extension: " + header;
                cout << error << endl;
                exit(0);
            }
        } else {
            string macro = string(headerItem->macroIdentifier);
            bool isUpperCase = all_of(macro.begin(), macro.end(), ::isupper);
            if (!isUpperCase) {
                string error = "Macro identifier is not all uppercase: " + macro;
                cout << error << endl;
                exit(0);
            } else {
                puts("Correct Define ID");
            }
        }
    }
}

void checkProgram(vector<Program *> *programList) {
    for (auto programItem : *programList) {
        if (programItem->isFunction == 0) {
            puts("Global Variable Declaration");
            checkDeclaration(programItem->declaration, nullptr);
        } else {
            puts("Function Declaration");
            checkFunctionDeclaration(programItem->functionDeclaration);
        }
    }
}

bool checkDeclarationInArguments(string declarationIdentifier, string dataType, vector<pair<string, string>> *argumentList) {
    for (auto argumentItem : *argumentList) {
        string argumentIdentifier = string(argumentItem.second);
        if (argumentIdentifier == declarationIdentifier) {
            string error = "Variable already declared as argument: " + declarationIdentifier;
            cout << error << endl;
            exit(0);
            return true;
        }
    }
    return false;
}

bool checkDeclarationInParentScope(string declarationIdentifier, string declarationScope, int checkFlag, GlobalSymTabEntry *functionEntry) {
    while (!declarationScope.empty()) {
        string declarationKey = declarationIdentifier + " <=> " + declarationScope;
        SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, declarationKey);
        if (entry != NULL) {
            string error = "Variable already declared: " + declarationIdentifier;
            cout << error << endl;
            exit(0);
            return true;
        }

        size_t lastDot = declarationScope.find_last_of(".");
        if (lastDot != string::npos) {
            declarationScope = declarationScope.substr(0, lastDot);
        } else {
            break;
        }
    }

    if (checkFlag == 1) {
        string globalDeclarationKey = declarationIdentifier;
        GlobalSymTabEntry *globalEntry = searchGlobalSymTab(root->globalSymbolTable, globalDeclarationKey);
        if (globalEntry != NULL) {
            string error = "Variable already declared in Global scope: " + declarationIdentifier;
            cout << error << endl;
            exit(0);
            return true;
        }
    }
    return false;
}

void checkDeclaration(Declaration *declaration, GlobalSymTabEntry *functionEntry) {
    string dataType = string(declaration->declarationType);
    vector<DeclarationIndex *> *declarationList = declaration->declarationList;
    string isInFunction = string(declaration->isInFunction);

    if (isInFunction == "Global") {
        for (auto declarationItem : *declarationList) {
            string declarationIdentifier = string(declarationItem->declarationIdentifier);
            string declarationKey = declarationIdentifier;
            if (declarationItem->flagDeclarationIndex == 0) {
                GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                if (entry == NULL) {
                    puts("Inserting undeclared variables into function local symbol table");
                    insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            } else if (declarationItem->flagDeclarationIndex == 1) {
                GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                if (entry == NULL) {
                    Expression *expression = declarationItem->expression;
                    string expressionType = checkExpression(expression, functionEntry);
                    if (expressionType != dataType) {
                        string error = "Type mismatch in declaration";
                        cout << error << endl;
                        exit(0);
                    }
                    puts("Inserting undeclared variables into function local symbol table");
                    insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            } else {
                GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                if (entry == NULL) {
                    vector<Expression *> *expressionList = declarationItem->expressionList;
                    for (auto expressionItem : *expressionList) {
                        string expressionType = checkExpression(expressionItem, functionEntry);
                        if (dataType == "point") {
                            if (expressionType != "num") {
                                string error = "Type mismatch in declaration";
                                cout << error << endl;
                                exit(0);
                            }
                        } else if (dataType == "line") {

                        } else if (dataType == "circle") {

                        } else if (dataType == "ellipse") {
                        } else {
                            string error = "Invalid data type in declaration";
                            cout << error << endl;
                            exit(0);
                        }
                    }
                    puts("Inserting undeclared variables into function local symbol table");
                    insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            }
        }
    } else {
        for (auto declarationItem : *declarationList) {
            string declarationIdentifier = string(declarationItem->declarationIdentifier);
            string declarationscope = string(declarationItem->scope);
            if (declarationItem->flagDeclarationIndex == 0) {
                bool matchFound = checkDeclarationInArguments(declarationIdentifier, dataType, functionEntry->arguments);
                if (!matchFound) {
                    bool matchFoundInParentScope = checkDeclarationInParentScope(declarationIdentifier, declarationscope, 0, functionEntry);
                    if (!matchFoundInParentScope) {
                        puts("Inserting undeclared variables into function local symbol table");
                        insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                    } else {
                        string error = "Variable already declared in parent scope: " + declarationIdentifier;
                        cout << error << endl;
                        exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            } else if (declarationItem->flagDeclarationIndex == 1) {
                bool matchFound = checkDeclarationInArguments(declarationIdentifier, dataType, functionEntry->arguments);
                if (!matchFound) {
                    bool matchFoundInParentScope = checkDeclarationInParentScope(declarationIdentifier, declarationscope, 0, functionEntry);
                    if (!matchFoundInParentScope) {
                        Expression *expression = declarationItem->expression;
                        string expressionType = checkExpression(expression, functionEntry);
                        if (expressionType != dataType) {
                            string error = "Type mismatch in declaration";
                            cout << error << endl;
                            exit(0);
                        }
                        puts("Inserting undeclared variables into function local symbol table");
                        insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                    } else {
                        string error = "Variable already declared in parent scope: " + declarationIdentifier;
                        cout << error << endl;
                        exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            } else {
                bool matchFound = checkDeclarationInArguments(declarationIdentifier, dataType, functionEntry->arguments);
                if (!matchFound) {
                    bool matchFoundInParentScope = checkDeclarationInParentScope(declarationIdentifier, declarationscope, 0, functionEntry);
                    if (!matchFoundInParentScope) {
                        vector<Expression *> *expressionList = declarationItem->expressionList;
                        for (auto expressionItem : *expressionList) {
                            string expressionType = checkExpression(expressionItem, functionEntry);
                            if (dataType == "point") {
                                if (expressionType != "num") {
                                    string error = "Type mismatch in declaration";
                                    cout << error << endl;
                                    exit(0);
                                }
                            } else if (dataType == "line") {

                            } else if (dataType == "circle") {

                            } else if (dataType == "ellipse") {
                            } else {
                                string error = "Invalid data type in declaration";
                                cout << error << endl;
                                exit(0);
                            }
                        }
                        puts("Inserting undeclared variables into function local symbol table");
                        insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                    } else {
                        string error = "Variable already declared in parent scope: " + declarationIdentifier;
                        cout << error << endl;
                        exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    exit(0);
                }
            }
        }
    }
}

void checkFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
    string returnType = string(functionDeclaration->returnType);
    string functionIdentifier = string(functionDeclaration->functonIdentifier);
    vector<FunctionArgumentList *> *argumentList = functionDeclaration->argumentList;
    CompoundStatement *compoundStatement = functionDeclaration->compoundStatement;

    vector<pair<string, string>> *arguments = new vector<pair<string, string>>;
    for (auto argumentItem : *argumentList) {
        string argumentType = string(argumentItem->ArgType);
        string argumentIdentifier = string(argumentItem->ArgIdentifier);
        arguments->push_back(make_pair(argumentType, argumentIdentifier));
    }

    string functionKey = functionIdentifier;
    cout << "Searching functionKey in Global symbol table: " << functionKey << endl;

    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, functionKey);
    if (entry == NULL) {
        puts("Inserting function into Global symbol table");
        insertGlobalSymTab(root->globalSymbolTable, functionIdentifier, returnType, "Function", "", arguments, functionDeclaration->row, functionDeclaration->column);
    } else {
        string error = "Function already declared: " + functionIdentifier;
        cout << error << endl;
        exit(0);
    }

    GlobalSymTabEntry *functionEntry = searchGlobalSymTab(root->globalSymbolTable, functionKey);
    checkCompoundStatement(compoundStatement, functionEntry);
}

void inOutStatement(InOut *inOut, GlobalSymTabEntry *functionEntry) {
    if (inOut->isWrite == 0) {
        vector<Scan *> *scanList = inOut->scanList;
        for (auto scanItem : *scanList) {
            string scanIdentifier = string(scanItem->scanIdentifier);
            string scanScope = scanItem->scope;
            bool matchFound = checkDeclarationInParentScope(scanIdentifier, scanScope, 1, functionEntry);
            if (!matchFound) {
                string error = "Variable not declared: " + scanIdentifier;
                cout << error << endl;
                exit(0);
            } else {
                puts("Variable Scanned");
            }
        }
    } else {
        vector<Print *> *printList = inOut->printList;
        for (auto printItem : *printList) {
            if (printItem->flagPrint == 0) {
                string printIdentifier = string(printItem->printIdentifier);
                string printScope = printItem->scope;
                bool matchFound = checkDeclarationInParentScope(printIdentifier, printScope, 1, functionEntry);
                if (!matchFound) {
                    string error = "Variable not declared: " + printIdentifier;
                    cout << error << endl;
                    exit(0);
                } else {
                    puts("Variable Printed");
                }
            } else {
                Expression *expression = printItem->expression;
                checkExpression(expression, functionEntry);
            }
        }
    }
}

string checkAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry) {
    PostfixExpression *postfixExpression = assignmentExpression->postfixExpression;
    Expression *expression = assignmentExpression->expression;
    if (postfixExpression != NULL) {
        string lhsType = checkPostfixExpression(postfixExpression, functionEntry);
        string rhsType = checkExpression(expression, functionEntry);
        if (lhsType != rhsType) {
            string error = "Type mismatch in assignment expression";
            cout << error << endl;
            exit(0);
        }
    } else {
        return checkExpression(expression, functionEntry);
    }
    return 0;
}

string checkPostfixExpression(PostfixExpression *postfixExpression, GlobalSymTabEntry *functionEntry) {
    if (postfixExpression->flagExpression == 0) {
        return checkBasicExpression(postfixExpression->basicExpression, functionEntry);
    } else if (postfixExpression->flagExpression == 1) {
        return checkFunctionCall(postfixExpression->functionCall, functionEntry);
    } else if (postfixExpression->flagExpression == 2) {

    } else if (postfixExpression->flagExpression == 3) {

    } else {
    }
}

string checkBasicExpression(BasicExpression *basicExpression, GlobalSymTabEntry *functionEntry) {
    if (basicExpression->flagBasic == 0) {
        return checkConstantValue(basicExpression->constantValue);
    } else if (basicExpression->flagBasic == 1) {
        string basicIdentifier = string(basicExpression->basicIdentifier);
        string scope = basicExpression->scope;
        bool matchFound = checkDeclarationInParentScope(basicIdentifier, scope, 1, functionEntry);
        if (!matchFound) {
            string error = "Variable not declared: " + basicIdentifier;
            cout << error << endl;
            exit(0);
        } else {
            puts("Variable found");
        }
    } else if (basicExpression->flagBasic == 2) {
        return checkExpression(basicExpression->expression, functionEntry);
    } else {
    }
}

string checkConstantValue(ConstantValue *constantValue) {
    if (constantValue->flagConstant == 0) {
        return "num";
    } else if (constantValue->flagConstant == 1) {
        return "boolean";
    } else if (constantValue->flagConstant == 2) {
        return "string";
    } else {
        return "null";
    }
}

string checkFunctionCall(FunctionCall *functionCall, GlobalSymTabEntry *functionEntry) {
    string functionCallIdentifier = string(functionCall->functionCallIdentifier);
    vector<Expression *> *argumentList = functionCall->argumentList;
    string functionCallKey = functionCallIdentifier;
    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, functionCallKey);
    if (entry == NULL) {
        string error = "Function not declared: " + functionCallIdentifier;
        cout << error << endl;
        exit(0);
    } else {
        puts("Function declared");
        vector<pair<string, string>> *arguments = entry->arguments;
        if (arguments->size() != argumentList->size()) {
            string error = "Argument size mismatch in function call: " + functionCallIdentifier;
            cout << error << endl;
            exit(0);
        } else {
            for (int i = 0; i < arguments->size(); i++) {
                string argumentDefaultType = arguments->at(i).first;
                string argumentCallType = checkExpression(argumentList->at(i), functionEntry);
                if (argumentDefaultType != argumentCallType) {
                    string error = "Argument type mismatch in function call: " + functionCallIdentifier + " at position " + to_string(i + 1);
                    cout << error << endl;
                    exit(0);
                    break;
                }
            }
        }
    }
    return 0;
}

string checkExpression(Expression *expression, GlobalSymTabEntry *functionEntry) {
    if (expression->flagExpression == 0) {
        UnaryExpression *unaryExpression = static_cast<UnaryExpression *>(expression);
        return checkUnaryExpression(unaryExpression, functionEntry);
    } else if (expression->flagExpression == 1) {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        return checkBinaryExpression(binaryExpression->lhs, binaryExpression->rhs, binaryExpression->op, functionEntry);
    }
}

string checkUnaryExpression(UnaryExpression *unaryExpression, GlobalSymTabEntry *functionEntry) {
    PostfixExpression *postfixExpression = unaryExpression->postfixExpression;
    vector<UnaryOperator> *opList = unaryExpression->opList;
    string postfixType = checkPostfixExpression(postfixExpression, functionEntry);
    for (auto op : *opList) {
        if (op == UnaryOperator::inc_op or op == UnaryOperator::dec_op) {
            if (postfixType != "num") {
                string error = "Type mismatch in unary expression";
                cout << error << endl;
                exit(0);
            }
        } else if (op == UnaryOperator::not_op) {
            if (postfixType != "boolean") {
                string error = "Type mismatch in unary expression";
                cout << error << endl;
                exit(0);
            }
        } else {
            string error = "Invalid unary operator";
            cout << error << endl;
            exit(0);
        }
    }
    return postfixType;
}

string checkBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry) {
    string lhsType = checkExpression(lhs, functionEntry);
    string rhsType = checkExpression(rhs, functionEntry);
    if (lhsType != rhsType) {
        string error = "Type mismatch in binary expression";
        cout << error << endl;
        exit(0);
    }
    return lhsType;
}

void checkCompoundStatement(CompoundStatement *compoundStatement, GlobalSymTabEntry *functionEntry) {
    vector<Statement *> *statementList = compoundStatement->statementList;
    for (auto statementItem : *statementList) {
        if (statementItem->flagStatement == 1) {
            cout << "Declaration inside function " << functionEntry->name << endl;
            checkDeclaration(statementItem->declaration, functionEntry);
        } else if (statementItem->flagStatement == 2) {
            cout << "InOut inside function " << functionEntry->name << endl;
            inOutStatement(statementItem->inOut, functionEntry);
        } else if (statementItem->flagStatement == 3) {
            cout << "ConditionalStatement inside function " << functionEntry->name << endl;
            checkConditionalStatement(statementItem->conditionalStatement, functionEntry);
        } else if (statementItem->flagStatement == 4) {
            cout << "JumpStatement inside function " << functionEntry->name << endl;
            checkJumpStatement(statementItem->jumpStatement, functionEntry);
        } else if (statementItem->flagStatement == 5) {
            cout << "CompoundStatement inside function " << functionEntry->name << endl;
            checkCompoundStatement(statementItem->compoundStatement, functionEntry);
        } else if (statementItem->flagStatement == 6) {
            cout << "IterativeStatement inside function " << functionEntry->name << endl;
            checkIterativeStatement(statementItem->iterativeStatement, functionEntry);
        } else {
            cout << "AssignmentExpression inside function " << functionEntry->name << endl;
            checkAssignmentExpression(statementItem->assignmentExpression, functionEntry);
        }
    }
}

void checkConditionalStatement(ConditionalStatement *conditionalStatement, GlobalSymTabEntry *functionEntry) {
    Expression *expression = conditionalStatement->expression;
    CompoundStatement *compoundStatement1 = conditionalStatement->compoundStatement1;
    vector<ElseIf *> *elseIfList = conditionalStatement->elseIfList;
    CompoundStatement *compoundStatement2 = conditionalStatement->compoundStatement2;

    string expressionType = checkExpression(expression, functionEntry);
    if (expressionType != "boolean") {
        string error = "Type mismatch in conditional statement";
        cout << error << endl;
        exit(0);
    }
    checkCompoundStatement(compoundStatement1, functionEntry);

    for (auto elseIfItem : *elseIfList) {
        Expression *expression = elseIfItem->expression;
        CompoundStatement *compoundStatement = elseIfItem->compoundStatement;
        string expressionType = checkExpression(expression, functionEntry);
        if (expressionType != "boolean") {
            string error = "Type mismatch in conditional statement";
            cout << error << endl;
            exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    }

    if (compoundStatement2 != NULL) {
        checkCompoundStatement(compoundStatement2, functionEntry);
    }
}

void checkIterativeStatement(IterativeStatement *iterativeStatement, GlobalSymTabEntry *functionEntry) {
    if (iterativeStatement->isWhile == 0) {
        Expression *expression1 = iterativeStatement->expression1;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        checkExpression(expression1, functionEntry);
        checkCompoundStatement(compoundStatement, functionEntry);
    } else if (iterativeStatement->isWhile == 1) {
        Declaration *declaration = iterativeStatement->declaration;
        Expression *expression2 = iterativeStatement->expression2;
        Expression *expression3 = iterativeStatement->expression3;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        checkDeclaration(declaration, functionEntry);
        checkExpression(expression2, functionEntry);
        checkExpression(expression3, functionEntry);
        checkCompoundStatement(compoundStatement, functionEntry);
    } else if (iterativeStatement->isWhile == 2) {
        Expression *expression1 = iterativeStatement->expression1;
        Expression *expression2 = iterativeStatement->expression2;
        Expression *expression3 = iterativeStatement->expression3;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        checkExpression(expression1, functionEntry);
        checkExpression(expression2, functionEntry);
        checkExpression(expression3, functionEntry);
        checkCompoundStatement(compoundStatement, functionEntry);
    }
}

void checkJumpStatement(JumpStatement *jumpStatement, GlobalSymTabEntry *functionEntry) {
    if (jumpStatement->flagJump == 2) {
        Expression *expression = jumpStatement->expression;
        string expressionType = checkExpression(expression, functionEntry);
        if (expressionType != functionEntry->dataType) {
            string error = "Type mismatch in return statement";
            cout << error << endl;
            exit(0);
        }
    }
}