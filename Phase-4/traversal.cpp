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

void checkHeader(vector<Header *> *headerList) {
    puts("Checking Header");
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
                // exit(0);
            }
        } else {
            string macro = string(headerItem->macroIdentifier);
            bool isUpperCase = all_of(macro.begin(), macro.end(), ::isupper);
            if (!isUpperCase) {
                string error = "Macro identifier is not all uppercase: " + macro;
                cout << error << endl;
                // exit(0);
            } else {
                puts("Correct Macro Identifier");
            }
        }
    }
}

void checkProgram(vector<Program *> *programList) {
    puts("Checking Program");
    for (auto programItem : *programList) {
        if (programItem->isFunction == 0) {
            checkDeclaration(programItem->declaration, nullptr);
        } else {
            checkFunctionDeclaration(programItem->functionDeclaration);
        }
    }
}

bool checkDeclarationInArguments(string declarationIdentifier, string dataType, vector<pair<string, string>> *argumentList) {
    puts("Checking Declaration in Arguments");
    for (auto argumentItem : *argumentList) {
        string argumentIdentifier = string(argumentItem.second);
        if (argumentIdentifier == declarationIdentifier) {
            // string error = "Variable already declared as argument: " + declarationIdentifier;
            // cout << error << endl;
            // exit(0);
            return true;
        }
    }
    return false;
}

string checkDeclarationInParentScope(string declarationIdentifier, string declarationScope, int checkFlag, GlobalSymTabEntry *functionEntry) {
    puts("Checking Declaration in Parent Scope");
    while (!declarationScope.empty()) {
        string declarationKey = declarationIdentifier + " <=> " + declarationScope;
        SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, declarationKey);
        if (entry != NULL) {
            string error = "Variable " + declarationIdentifier + " found in scope: " + declarationScope;
            cout << error << endl;
            return entry->dataType;
            // exit(0);
        }

        size_t lastDot = declarationScope.find_last_of(".");
        if (lastDot != string::npos) {
            declarationScope = declarationScope.substr(0, lastDot);
        } else {
            break;
        }
    }

    if (functionEntry != nullptr) {
        for (auto argument : *functionEntry->arguments) {
            if (argument.second == declarationIdentifier) {
                string error = "Variable " + declarationIdentifier + " found in function arguments";
                cout << error << endl;
                return argument.first;
            }
        }
    }

    if (checkFlag == 1) {
        string globalDeclarationKey = declarationIdentifier;
        GlobalSymTabEntry *globalEntry = searchGlobalSymTab(root->globalSymbolTable, globalDeclarationKey);
        if (globalEntry != NULL) {
            string error = "Variable " + declarationIdentifier + " found in Global scope";
            cout << error << endl;
            return globalEntry->dataType;
            // exit(0);
        }
    }
    return "";
}

bool checkTypeInDeclaration(string dataType, vector<Expression *> *expressionList, GlobalSymTabEntry *functionEntry) {
    if (dataType == "point") {
        if (expressionList->size() != 2) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            Expression *expressionItem1 = expressionList->at(0);
            Expression *expressionItem2 = expressionList->at(1);
            string expressionType1 = checkExpression(expressionItem1, functionEntry);
            string expressionType2 = checkExpression(expressionItem2, functionEntry);
            if (expressionType1 == "num" && expressionType2 == "num") {
                return true;
            } else {
                string error = "Type mismatch in declaration";
                cout << error << endl;
                return false;
                // exit(0);
            }
        }
    } else if (dataType == "line") {
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            if (expressionList->size() == 2) {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                if ((expressionType1 == "point" && expressionType2 == "point") || (expressionType1 == "num" && expressionType2 == "num")) {
                    return true;
                } else {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            } else {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                } else {
                    return true;
                }
            }
        }
    } else if (dataType == "curve") {
        if (expressionList->size() != 6) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            for (auto expressionItem : *expressionList) {
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "num") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            }
            return true;
        }
    } else if (dataType == "circle") {
        cout << expressionList->size() << endl;
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            string error = "Invalid number of rutwik arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                } else {
                    return true;
                }
            } else {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                if (expressionType1 == "point" && expressionType2 == "num") {
                    return true;
                } else {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            }
        }
    } else if (dataType == "parabola") {
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                } else {
                    return true;
                }
            } else {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                if (expressionType1 == "point" && expressionType2 == "point") {
                    return true;
                } else {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            }
        }
    } else if (dataType == "ellipse") {
        if (expressionList->size() != 3 && expressionList->size() != 1) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                } else {
                    return true;
                }
            } else {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                Expression *expressionItem3 = expressionList->at(2);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                string expressionType3 = checkExpression(expressionItem3, functionEntry);
                if (expressionType1 == "point" && expressionType2 == "num" && expressionType3 == "num") {
                    return true;
                } else {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            }
        }
    } else if (dataType == "hyperbola") {
        if (expressionList->size() != 3 && expressionList->size() != 1) {
            string error = "Invalid number of arguments in declaration";
            cout << error << endl;
            return false;
            // exit(0);
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                } else {
                    return true;
                }
            } else {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                Expression *expressionItem3 = expressionList->at(2);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                string expressionType3 = checkExpression(expressionItem3, functionEntry);
                if (expressionType1 == "point" && expressionType2 == "num" && expressionType3 == "num") {
                    return true;
                } else {
                    string error = "Type mismatch in declaration";
                    cout << error << endl;
                    return false;
                    // exit(0);
                }
            }
        }
    } else {
        string error = "Invalid data type in declaration";
        cout << error << endl;
        return false;
        // exit(0);
    }
}

void checkDeclaration(Declaration *declaration, GlobalSymTabEntry *functionEntry) {
    if (functionEntry == nullptr) {
        puts("Check Declaration in Global Scope");
    } else {
        puts("Check Declaration in Function Scope");
    }
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
                    cout << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                    insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
                }
            } else if (declarationItem->flagDeclarationIndex == 1) {
                GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                if (entry == NULL) {
                    Expression *expression = declarationItem->expression;
                    string expressionType = checkExpression(expression, functionEntry);
                    if (expressionType != dataType) {
                        string error = "Type mismatch in declaration <-> " + dataType + " " + expressionType;
                        cout << error << endl;
                        // exit(0);
                    } else {
                        cout << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                        insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                    }
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
                }
            } else {
                GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                if (entry == NULL) {
                    vector<Expression *> *expressionList = declarationItem->expressionList;
                    bool typeCheck = checkTypeInDeclaration(dataType, expressionList, functionEntry);
                    if (typeCheck) {
                        cout << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                        insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                    }
                } else {
                    string error = "Variable already declared in Global scope: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
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
                    string idKey = declarationIdentifier + " <=> " + declarationscope;
                    SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, idKey);
                    if (entry == nullptr) {
                        cout << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                        insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                    } else {
                        string error = "Variable already declared in respective scope: " + declarationIdentifier;
                        cout << error << endl;
                        // exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
                }
            } else if (declarationItem->flagDeclarationIndex == 1) {
                bool matchFound = checkDeclarationInArguments(declarationIdentifier, dataType, functionEntry->arguments);
                if (!matchFound) {
                    string idKey = declarationIdentifier + " <=> " + declarationscope;
                    SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, idKey);
                    if (entry == nullptr) {
                        Expression *expression = declarationItem->expression;
                        string expressionType = checkExpression(expression, functionEntry);
                        if (expressionType != dataType) {
                            string error = "Type mismatch in declaration";
                            cout << error << endl;
                            // exit(0);
                        } else {
                            cout << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                            insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                        }
                    } else {
                        string error = "Variable already declared in respective scope: " + declarationIdentifier;
                        cout << error << endl;
                        // exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
                }
            } else {
                bool matchFound = checkDeclarationInArguments(declarationIdentifier, dataType, functionEntry->arguments);
                if (!matchFound) {
                    string idKey = declarationIdentifier + " <=> " + declarationscope;
                    SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, idKey);
                    if (entry == nullptr) {
                        vector<Expression *> *expressionList = declarationItem->expressionList;
                        bool typeCheck = checkTypeInDeclaration(dataType, expressionList, functionEntry);
                        if (typeCheck) {
                            cout << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                            insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                        }
                    } else {
                        string error = "Variable already declared in respective scope: " + declarationIdentifier;
                        cout << error << endl;
                        // exit(0);
                    }
                } else {
                    string error = "Variable already declared as argument: " + declarationIdentifier;
                    cout << error << endl;
                    // exit(0);
                }
            }
        }
    }
}

void checkFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
    puts("Function Declaration");
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
        // exit(0);
    }

    GlobalSymTabEntry *functionEntry = searchGlobalSymTab(root->globalSymbolTable, functionKey);
    checkCompoundStatement(compoundStatement, functionEntry);
}

void inOutStatement(InOut *inOut, GlobalSymTabEntry *functionEntry) {
    puts("InOut Statement");
    if (inOut->isWrite == 0) {
        vector<Scan *> *scanList = inOut->scanList;
        for (auto scanItem : *scanList) {
            string scanIdentifier = string(scanItem->scanIdentifier);
            string scanScope = scanItem->scope;
            string matchFound = checkDeclarationInParentScope(scanIdentifier, scanScope, 1, functionEntry);
            if (matchFound == "") {
                string error = "Variable not declared: " + scanIdentifier;
                cout << error << endl;
                // exit(0);
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
                string matchFound = checkDeclarationInParentScope(printIdentifier, printScope, 1, functionEntry);
                if (matchFound == "") {
                    string error = "Variable not declared: " + printIdentifier;
                    cout << error << endl;
                    // exit(0);
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

void checkAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry) {
    puts("Assignment Expression");
    PostfixExpression *postfixExpression = assignmentExpression->postfixExpression;
    Expression *expression = assignmentExpression->expression;
    if (postfixExpression != NULL) {
        string lhsType = checkPostfixExpression(postfixExpression, functionEntry);
        string rhsType = checkExpression(expression, functionEntry);
        if (lhsType != rhsType) {
            string error = "Type mismatch in assignment expression in " + functionEntry->name;
            cout << error << endl;
            // exit(0);
        } else {
            // cout << lhsType << " " << rhsType << endl;
        }
    } else {
        checkExpression(expression, functionEntry);
    }
}

string checkPostfixExpression(PostfixExpression *postfixExpression, GlobalSymTabEntry *functionEntry) {
    puts("Postfix Expression");
    if (postfixExpression->flagPostfix == 0) {
        return checkBasicExpression(postfixExpression->basicExpression, functionEntry);
    } else if (postfixExpression->flagPostfix == 1) {
        return checkFunctionCall(postfixExpression->functionCall, functionEntry);
    } else if (postfixExpression->flagPostfix == 2) {
        string postfixIdentifier = string(postfixExpression->postfixIdentifier);
        string scope = postfixExpression->scope;
        string matchFound = checkDeclarationInParentScope(postfixIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            string error = "Variable not declared: " + postfixIdentifier;
            cout << error << endl;
            return "";
            // exit(0);
        } else {
            if (matchFound == "point") {
                if (postfixExpression->memberVariable1 == MemberVariable::x || postfixExpression->memberVariable1 == MemberVariable::y) {
                    return "num";
                } else {
                    string error = "Invalid member variable in point";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "line") {
                if (postfixExpression->memberVariable1 == MemberVariable::slope || postfixExpression->memberVariable1 == MemberVariable::c) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    string error = "Invalid member variable in line";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "circle") {
                if (postfixExpression->memberVariable1 == MemberVariable::radius || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    string error = "Invalid member variable in circle";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "ellipse") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    string error = "Invalid member variable in ellipse";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "parabola") {
                if (postfixExpression->memberVariable1 == MemberVariable::vertex || postfixExpression->memberVariable1 == MemberVariable::focus) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::latus_rectum || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    string error = "Invalid member variable in parabola";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "hyperbola") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    string error = "Invalid member variable in hyperbola";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "curve") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::c || postfixExpression->memberVariable1 == MemberVariable::f || postfixExpression->memberVariable1 == MemberVariable::g || postfixExpression->memberVariable1 == MemberVariable::h || postfixExpression->memberVariable1 == MemberVariable::delta) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::equation || postfixExpression->memberVariable1 == MemberVariable::type) {
                    return "string";
                } else {
                    string error = "Invalid member variable in curve";
                    cout << error << endl;
                    return "";
                }
            } else {
                string error = "Invalid data type in postfix expression";
                cout << error << endl;
                return "";
            }
        }
    } else if (postfixExpression->flagPostfix == 3) {
        string postfixIdentifier = string(postfixExpression->postfixIdentifier);
        string scope = postfixExpression->scope;
        string matchFound = checkDeclarationInParentScope(postfixIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            string error = "Variable not declared: " + postfixIdentifier;
            cout << error << endl;
            return "";
            // exit(0);
        } else {
            if (matchFound == "line") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2);
                } else {
                    string error = "Invalid member variable in line";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "circle") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        string error = "Invalid member variable in circle";
                        cout << error << endl;
                        return "";
                    }
                } else {
                    string error = "Invalid member variable in circle";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "ellipse") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        string error = "Invalid member variable in ellipse";
                        cout << error << endl;
                        return "";
                    }
                } else {
                    string error = "Invalid member variable in ellipse";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "parabola") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2);
                } else if (postfixExpression->memberVariable1 == MemberVariable::vertex || postfixExpression->memberVariable1 == MemberVariable::focus) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        string error = "Invalid member variable in parabola";
                        cout << error << endl;
                        return "";
                    }
                } else {
                    string error = "Invalid member variable in parabola";
                    cout << error << endl;
                    return "";
                }
            } else if (matchFound == "hyperbola") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        string error = "Invalid member variable in hyperbola";
                        cout << error << endl;
                        return "";
                    }
                } else {
                    string error = "Invalid member variable in hyperbola";
                    cout << error << endl;
                    return "";
                }
            } else {
                string error = "Invalid data type in postfix expression";
                cout << error << endl;
                return "";
            }
        }
    }
}

string curveMemberAccess(MemberVariable memberVariable) {
    if (memberVariable == MemberVariable::a || memberVariable == MemberVariable::b || memberVariable == MemberVariable::c || memberVariable == MemberVariable::f || memberVariable == MemberVariable::g || memberVariable == MemberVariable::h || memberVariable == MemberVariable::delta) {
        return "num";
    } else if (memberVariable == MemberVariable::equation || memberVariable == MemberVariable::type) {
        return "string";
    } else {
        string error = "Invalid member variable in curve";
        cout << error << endl;
        return "";
    }
}

string checkBasicExpression(BasicExpression *basicExpression, GlobalSymTabEntry *functionEntry) {
    puts("Basic Expression");
    if (basicExpression->flagBasic == 0) {
        return checkConstantValue(basicExpression->constantValue);
    } else if (basicExpression->flagBasic == 1) {
        string basicIdentifier = string(basicExpression->basicIdentifier);
        string scope = basicExpression->scope;
        string matchFound = checkDeclarationInParentScope(basicIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            string error = "Variable not declared: " + basicIdentifier;
            cout << error << endl;
            return "";
            // exit(0);
        } else {
            return matchFound;
        }
    } else if (basicExpression->flagBasic == 2) {
        return checkExpression(basicExpression->expression, functionEntry);
    }
}

string checkConstantValue(ConstantValue *constantValue) {
    puts("Constant");
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
    puts("Function Call");
    string functionCallIdentifier = string(functionCall->functionCallIdentifier);
    vector<Expression *> *argumentList = functionCall->argumentList;
    string functionCallKey = functionCallIdentifier;
    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, functionCallKey);
    if (entry == NULL) {
        string error = "Function not declared: " + functionCallIdentifier;
        cout << error << endl;
        return "";
        // exit(0);
    } else {
        puts("Function declared");
        vector<pair<string, string>> *arguments = entry->arguments;
        if (arguments->size() != argumentList->size()) {
            string error = "Argument size mismatch in function call: " + functionCallIdentifier;
            cout << error << endl;
            return "";
            // exit(0);
        } else {
            for (int i = 0; i < arguments->size(); i++) {
                string argumentDefaultType = arguments->at(i).first;
                string argumentCallType = checkExpression(argumentList->at(i), functionEntry);
                if (argumentDefaultType != argumentCallType) {
                    string error = "Argument type mismatch in function call: " + functionCallIdentifier + " at position " + to_string(i + 1);
                    cout << error << endl;
                    return "";
                    // exit(0);
                    break;
                }
            }
        }
    }
    return 0;
}

string checkExpression(Expression *expression, GlobalSymTabEntry *functionEntry) {
    puts("Expression");
    if (expression->flagExpression == 0) {
        UnaryExpression *unaryExpression = static_cast<UnaryExpression *>(expression);
        return checkUnaryExpression(unaryExpression, functionEntry);
    } else if (expression->flagExpression == 1) {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        return checkBinaryExpression(binaryExpression->lhs, binaryExpression->rhs, binaryExpression->op, functionEntry);
    }
}

string checkUnaryExpression(UnaryExpression *unaryExpression, GlobalSymTabEntry *functionEntry) {
    puts("Unary Expression");
    PostfixExpression *postfixExpression = unaryExpression->postfixExpression;
    vector<UnaryOperator> *opList = unaryExpression->opList;
    string postfixType = checkPostfixExpression(postfixExpression, functionEntry);
    for (auto op : *opList) {
        if (op == UnaryOperator::inc_op or op == UnaryOperator::dec_op) {
            if (postfixType != "num") {
                string error = "Type mismatch in unary expression";
                cout << error << endl;
                return "";
                // exit(0);
            }
        } else if (op == UnaryOperator::not_op) {
            if (postfixType != "boolean") {
                string error = "Type mismatch in unary expression";
                cout << error << endl;
                return "";
                // exit(0);
            }
        } else {
            string error = "Invalid unary operator";
            cout << error << endl;
            return "";
            // exit(0);
        }
    }
    return postfixType;
}

string checkBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry) {
    puts("Binary Expression");
    string lhsType = checkExpression(lhs, functionEntry);
    string rhsType = checkExpression(rhs, functionEntry);
    if (lhsType != rhsType) {
        string error = "Type mismatch in binary expression";
        cout << error << endl;
        return "";
        // exit(0);
    } else {
        return lhsType;
    }
    return "";
}

void checkCompoundStatement(CompoundStatement *compoundStatement, GlobalSymTabEntry *functionEntry) {
    puts("In Compound Statement");
    vector<Statement *> *statementList = compoundStatement->statementList;
    for (auto statementItem : *statementList) {
        if (statementItem->flagStatement == 0) {
            cout << "AssignmentExpression inside function " << functionEntry->name << endl;
            checkAssignmentExpression(statementItem->assignmentExpression, functionEntry);
            cout << "AssignmentExpression done" << endl;
        } else if (statementItem->flagStatement == 1) {
            cout << "Declaration inside function " << functionEntry->name << endl;
            checkDeclaration(statementItem->declaration, functionEntry);
            cout << "Declaration done" << endl;
        } else if (statementItem->flagStatement == 2) {
            cout << "InOut inside function " << functionEntry->name << endl;
            inOutStatement(statementItem->inOut, functionEntry);
        } else if (statementItem->flagStatement == 3) {
            cout << "CompoundStatement inside function " << functionEntry->name << endl;
            checkCompoundStatement(statementItem->compoundStatement, functionEntry);
        } else if (statementItem->flagStatement == 4) {
            cout << "ConditionalStatement inside function " << functionEntry->name << endl;
            checkConditionalStatement(statementItem->conditionalStatement, functionEntry);
            cout << "ConditionalStatement done" << endl;
        } else if (statementItem->flagStatement == 5) {
            cout << "IterativeStatement inside function " << functionEntry->name << endl;
            checkIterativeStatement(statementItem->iterativeStatement, functionEntry);
        } else if (statementItem->flagStatement == 6) {
            cout << "JumpStatement inside function " << functionEntry->name << endl;
            checkJumpStatement(statementItem->jumpStatement, functionEntry);
        } else {
        }
    }
}

void checkConditionalStatement(ConditionalStatement *conditionalStatement, GlobalSymTabEntry *functionEntry) {
    puts("Conditional Statement");
    Expression *expression = conditionalStatement->expression;
    CompoundStatement *compoundStatement1 = conditionalStatement->compoundStatement1;
    vector<ElseIf *> *elseIfList = conditionalStatement->elseIfList;
    CompoundStatement *compoundStatement2 = conditionalStatement->compoundStatement2;

    string expressionType = checkConditionalExpression(expression, functionEntry);
    if (expressionType != "boolean") {
        string error = "Type mismatch in conditional statement";
        cout << error << endl;
        // exit(0);
    }
    checkCompoundStatement(compoundStatement1, functionEntry);

    for (auto elseIfItem : *elseIfList) {
        Expression *expression = elseIfItem->expression;
        CompoundStatement *compoundStatement = elseIfItem->compoundStatement;
        string expressionType = checkConditionalExpression(expression, functionEntry);
        if (expressionType != "boolean") {
            string error = "Type mismatch in conditional statement";
            cout << error << endl;
            // exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    }

    if (compoundStatement2 != NULL) {
        checkCompoundStatement(compoundStatement2, functionEntry);
    }
}

void checkIterativeStatement(IterativeStatement *iterativeStatement, GlobalSymTabEntry *functionEntry) {
    puts("Iterative Statement");
    if (iterativeStatement->isWhile == 0) {
        Expression *expression2 = iterativeStatement->expression2;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        string expressionType = checkConditionalExpression(expression2, functionEntry);
        if (expressionType != "boolean") {
            string error = "Type mismatch in while statement";
            cout << error << endl;
            // exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    } else if (iterativeStatement->isWhile == 1) {
        Declaration *declaration = iterativeStatement->declaration;
        Expression *expression2 = iterativeStatement->expression2;
        AssignmentExpression *expression3 = iterativeStatement->assignmentexpression3;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        checkDeclaration(declaration, functionEntry);
        string expressionType = checkConditionalExpression(expression2, functionEntry);
        if (expressionType != "boolean") {
            string error = "Type mismatch in for statement";
            cout << error << endl;
            // exit(0);
        }
        string assignmentType = checkIterativeAssignmentExpression(expression3, functionEntry);
        if (assignmentType != "Initialisation") {
            string error = "Type mismatch in for statement";
            cout << error << endl;
            // exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    } else if (iterativeStatement->isWhile == 2) {
        AssignmentExpression *expression1 = iterativeStatement->assignmentexpression1;
        Expression *expression2 = iterativeStatement->expression2;
        AssignmentExpression *expression3 = iterativeStatement->assignmentexpression3;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        string assignmentType1 = checkIterativeAssignmentExpression(expression1, functionEntry);
        if (assignmentType1 != "Initialisation") {
            string error = "Type mismatch in for statement";
            cout << error << endl;
            // exit(0);
        }
        string expressionType = checkConditionalExpression(expression2, functionEntry);
        if (expressionType != "boolean") {
            string error = "Type mismatch in for statement";
            cout << error << endl;
            // exit(0);
        }
        string assignmentType2 = checkIterativeAssignmentExpression(expression3, functionEntry);
        if (assignmentType2 != "Initialisation") {
            string error = "Type mismatch in for statement";
            cout << error << endl;
            // exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    }
}

void checkJumpStatement(JumpStatement *jumpStatement, GlobalSymTabEntry *functionEntry) {
    puts("Jump Statement");
    if (jumpStatement->flagJump == 2) {
        Expression *expression = jumpStatement->expression;
        string expressionType = checkExpression(expression, functionEntry);
        if (expressionType != functionEntry->dataType) {
            string error = "Type mismatch in return statement";
            cout << error << endl;
            // exit(0);
        }
    }
}

string checkConditionalExpression(Expression *expression, GlobalSymTabEntry *functionEntry) {
    puts("Conditional Expression");
    if (expression->flagExpression == 1) {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        return checkConditionalBinaryExpression(binaryExpression->lhs, binaryExpression->rhs, binaryExpression->op, functionEntry);
    }
}

string checkConditionalBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry) {
    puts("Conditional Binary Expression");
    string lhsType = checkExpression(lhs, functionEntry);
    string rhsType = checkExpression(rhs, functionEntry);
    if (lhsType != rhsType) {
        string error = "Type mismatch in conditional binary expression";
        cout << error << endl;
        return "";
        // exit(0);
    } else {
        if (op == BinaryOperator::equal_op or op == BinaryOperator::not_equal_op or op == BinaryOperator::less_equal_op or op == BinaryOperator::less_op or op == BinaryOperator::greater_equal_op or op == BinaryOperator::greater_op) {
            return "boolean";
        } else {
            string error = "Invalid operator in conditional binary expression";
            cout << error << endl;
            return "";
            // exit(0);
        }
    }
    return "";
}

string checkIterativeAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry) {
    puts("Iterative Assignment Expression");
    PostfixExpression *postfixExpression = assignmentExpression->postfixExpression;
    Expression *expression = assignmentExpression->expression;
    if (postfixExpression != NULL) {
        string lhsType = checkPostfixExpression(postfixExpression, functionEntry);
        string rhsType = checkExpression(expression, functionEntry);
        if (lhsType != rhsType) {
            string error = "Type mismatch in assignment expression in " + functionEntry->name;
            cout << error << endl;
            return "";
            // exit(0);
        } else {
            return "Initialisation";
        }
    }
}