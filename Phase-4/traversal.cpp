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
