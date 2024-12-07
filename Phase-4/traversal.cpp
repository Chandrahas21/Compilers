#include "traversal.hpp"
#include "error.hpp"
#include "symbolTable.hpp"
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Start *root;
#include <fstream>

ofstream out("Test/log.txt");

bool traversal(Start *start) {
    root = start;
    checkHeader(start->headerList);
    checkProgram(start->programList);
    return true;
}

void checkHeader(vector<Header *> *headerList) {
    out << "Checking Header ";
    for (auto headerItem : *headerList) {
        if (headerItem->isHeader == 1) {
            string header = string(headerItem->header);
            if (header.size() >= 2 && (header.substr(header.size() - 6) == ".core\"")) {
                out << "Correct Header.h" << endl;
            } else if (header.size() >= 4 && header.substr(header.size() - 7) == ".utils\"") {
                out << "Correct Header.hpp" << endl;
            } else {
                printError(INVALID_HEADER, headerItem->row, headerItem->column);
                exit(0);
            }
        } else {
            string macro = string(headerItem->macroIdentifier);
            bool isUpperCase = all_of(macro.begin(), macro.end(), ::isupper);
            if (!isUpperCase) {
                printError(INVALID_MACRO, headerItem->row, headerItem->column);
                exit(0);
            } else {
                out << "Correct Macro Identifier" << endl;
            }
        }
    }
}

void checkProgram(vector<Program *> *programList) {
    out << "Checking Program" << endl;
    for (auto programItem : *programList) {
        if (programItem->isFunction == 0) {
            checkDeclaration(programItem->declaration, nullptr);
        } else {
            checkFunctionDeclaration(programItem->functionDeclaration);
        }
    }
}

bool checkDeclarationInArguments(string declarationIdentifier, string dataType, vector<pair<string, string>> *argumentList) {
    out << "Checking " << declarationIdentifier << " in arguments" << endl;
    for (auto argumentItem : *argumentList) {
        string argumentIdentifier = string(argumentItem.second);
        if (argumentIdentifier == declarationIdentifier) {
            return true;
        }
    }
    return false;
}

string checkDeclarationInParentScope(string declarationIdentifier, string declarationScope, int checkFlag, GlobalSymTabEntry *functionEntry) {
    out << "Checking " << declarationIdentifier << " with " << declarationScope << " in " << functionEntry->name << endl;
    while (!declarationScope.empty()) {
        string declarationKey = declarationIdentifier + " <=> " + declarationScope;
        SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, declarationKey);
        if (entry != NULL) {
            string signal = "Variable " + declarationIdentifier + " found in scope: " + declarationScope;
            out << signal << endl;
            return entry->dataType;
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
                string signal = "Variable " + declarationIdentifier + " found in function arguments";
                out << signal << endl;
                return argument.first;
            }
        }
    }

    if (checkFlag == 1) {
        string globalDeclarationKey = declarationIdentifier;
        GlobalSymTabEntry *globalEntry = searchGlobalSymTab(root->globalSymbolTable, globalDeclarationKey);
        if (globalEntry != NULL) {
            string signal = "Variable " + declarationIdentifier + " found in Global scope";
            out << signal << endl;
            return globalEntry->dataType;
        }
    }

    return "";
}

bool checkTypeInDeclaration(string dataType, vector<Expression *> *expressionList, GlobalSymTabEntry *functionEntry) {
    out << "Checking type in declaration" << endl;
    if (dataType == "point") {
        if (expressionList->size() != 2) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            Expression *expressionItem1 = expressionList->at(0);
            Expression *expressionItem2 = expressionList->at(1);
            string expressionType1 = checkExpression(expressionItem1, functionEntry);
            string expressionType2 = checkExpression(expressionItem2, functionEntry);
            if (expressionType1 == "num" && expressionType2 == "num") {
                return true;
            } else {
                printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                return false;
            }
        }
    } else if (dataType == "line") {
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            if (expressionList->size() == 2) {
                Expression *expressionItem1 = expressionList->at(0);
                Expression *expressionItem2 = expressionList->at(1);
                string expressionType1 = checkExpression(expressionItem1, functionEntry);
                string expressionType2 = checkExpression(expressionItem2, functionEntry);
                if ((expressionType1 == "point" && expressionType2 == "point") || (expressionType1 == "num" && expressionType2 == "num")) {
                    return true;
                } else {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                }
            } else {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                } else {
                    return true;
                }
            }
        }
    } else if (dataType == "curve") {
        if (expressionList->size() != 6) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            for (auto expressionItem : *expressionList) {
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "num") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionItem->row, expressionItem->column);
                    return false;
                }
            }
            return true;
        }
    } else if (dataType == "circle") {
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
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
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                }
            }
        }
    } else if (dataType == "parabola") {
        if (expressionList->size() != 2 && expressionList->size() != 1) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
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
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                }
            }
        }
    } else if (dataType == "ellipse") {
        if (expressionList->size() != 3 && expressionList->size() != 1) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
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
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                }
            }
        }
    } else if (dataType == "hyperbola") {
        if (expressionList->size() != 3 && expressionList->size() != 1) {
            printError(INVALID_NO_OF_ARGS_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
            return false;
        } else {
            if (expressionList->size() == 1) {
                Expression *expressionItem = expressionList->at(0);
                string expressionType = checkExpression(expressionItem, functionEntry);
                if (expressionType != "curve") {
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
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
                    printError(TYPE_MISMATCH_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
                    return false;
                }
            }
        }
    } else {
        printError(INVALID_DECLARATION, expressionList->at(0)->row, expressionList->at(0)->column);
        return false;
    }
}

void checkDeclaration(Declaration *declaration, GlobalSymTabEntry *functionEntry) {
    if (functionEntry == nullptr) {
        out << "Check Declaration in Global Scope" << endl;
    } else {
        out << "Check Declaration in " << functionEntry->name << endl;
    }
    string dataType = string(declaration->declarationType);
    vector<DeclarationIndex *> *declarationList = declaration->declarationList;
    string isInFunction = string(declaration->isInFunction);

    if (declaration->isVector == 0) {
        if (isInFunction == "Global") {
            for (auto declarationItem : *declarationList) {
                string declarationIdentifier = string(declarationItem->declarationIdentifier);
                string declarationKey = declarationIdentifier;
                if (declarationItem->flagDeclarationIndex == 0) {
                    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                    if (entry == NULL) {
                        out << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                        insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                    } else {
                        printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                        exit(0);
                    }
                } else if (declarationItem->flagDeclarationIndex == 1) {
                    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                    if (entry == NULL) {
                        Expression *expression = declarationItem->expression;
                        string expressionType = checkExpression(expression, functionEntry);
                        if (expressionType != dataType) {
                            printError(TYPE_MISMATCH_DECLARATION, declarationItem->row, declarationItem->column);
                            exit(0);
                        } else {
                            out << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                            insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                        exit(0);
                    }
                } else {
                    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                    if (entry == NULL) {
                        vector<Expression *> *expressionList = declarationItem->expressionList;
                        bool typeCheck = checkTypeInDeclaration(dataType, expressionList, functionEntry);
                        if (typeCheck) {
                            out << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                            insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Variable", "", NULL, declarationItem->row, declarationItem->column);
                        } else {
                            // Already printed in function if it returns false
                            exit(0);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
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
                        string idKey = declarationIdentifier + " <=> " + declarationscope;
                        SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, idKey);
                        if (entry == nullptr) {
                            out << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                            insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                        } else {
                            printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                            exit(0);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION_FUNC_ARGUMENT, declarationItem->row, declarationItem->column);
                        exit(0);
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
                                printError(TYPE_MISMATCH_DECLARATION, declarationItem->row, declarationItem->column);
                                exit(0);
                            } else {
                                out << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                                insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                            }
                        } else {
                            printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                            exit(0);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION_FUNC_ARGUMENT, declarationItem->row, declarationItem->column);
                        exit(0);
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
                                out << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                                insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                            } else {
                                exit(0);
                            }
                        } else {
                            printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                            exit(0);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION_FUNC_ARGUMENT, declarationItem->row, declarationItem->column);
                        exit(0);
                    }
                }
            }
        }
    } else if (declaration->isVector == 1) {
        if (declaration->isInFunction == "Global") {
            for (auto declarationItem : *declarationList) {
                string declarationIdentifier = string(declarationItem->declarationIdentifier);
                string declarationKey = declarationIdentifier;
                if (declarationItem->flagDeclarationIndex == 0) {
                    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, declarationKey);
                    if (entry == NULL) {
                        out << "Inserting variable into Global symbol table: " << declarationIdentifier << endl;
                        insertGlobalSymTab(root->globalSymbolTable, declarationIdentifier, dataType, "Vector", "", NULL, declarationItem->row, declarationItem->column);
                    } else {
                        printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
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
                        string idKey = declarationIdentifier + " <=> " + declarationscope;
                        SymTabEntry *entry = searchLocalSymTab(functionEntry->symbolTable, idKey);
                        if (entry == nullptr) {
                            out << "Inserting variable into function local symbol table: " << declarationIdentifier << " in " << functionEntry->name << endl;
                            insertLocalSymTab(functionEntry->symbolTable, declarationIdentifier, dataType, declarationscope, declarationItem->row, declarationItem->column);
                        } else {
                            printError(VARIABLE_REDECLARATION, declarationItem->row, declarationItem->column);
                            exit(0);
                        }
                    } else {
                        printError(VARIABLE_REDECLARATION_FUNC_ARGUMENT, declarationItem->row, declarationItem->column);
                        exit(0);
                    }
                }
            }
        }
    }
}

void checkFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
    out << "Checking Function Declaration" << endl;
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

    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, functionKey);
    if (entry == NULL) {
        out << "Inserting function into Global symbol table: " << functionIdentifier << endl;
        insertGlobalSymTab(root->globalSymbolTable, functionIdentifier, returnType, "Function", "", arguments, functionDeclaration->row, functionDeclaration->column);
    } else {
        printError(FUNCTION_REDECLARATION, functionDeclaration->row, functionDeclaration->column);
        exit(0);
    }

    GlobalSymTabEntry *functionEntry = searchGlobalSymTab(root->globalSymbolTable, functionKey);
    checkCompoundStatement(compoundStatement, functionEntry);
}

void inOutStatement(InOut *inOut, GlobalSymTabEntry *functionEntry) {
    out << "Checking InOut Statement" << endl;
    if (inOut->isWrite == 0) {
        vector<Scan *> *scanList = inOut->scanList;
        for (auto scanItem : *scanList) {
            string scanIdentifier = string(scanItem->scanIdentifier);
            string scanScope = scanItem->scope;
            string matchFound = checkDeclarationInParentScope(scanIdentifier, scanScope, 1, functionEntry);
            if (matchFound == "") {
                printError(VARIABLE_NOT_DECLARED, scanItem->row, scanItem->column);
                exit(0);
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
                    printError(VARIABLE_NOT_DECLARED, printItem->row, printItem->column);
                    exit(0);
                }
            } else {
                Expression *expression = printItem->expression;
                checkExpression(expression, functionEntry);
            }
        }
    }
}

void checkAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Assignment Expression" << endl;
    PostfixExpression *postfixExpression = assignmentExpression->postfixExpression;
    Expression *expression = assignmentExpression->expression;
    if (postfixExpression != NULL) {
        string lhsType = checkPostfixExpression(postfixExpression, functionEntry);
        string rhsType = checkExpression(expression, functionEntry);
        if (lhsType != rhsType) {
            printError(TYPE_MISMATCH_ASSIGNEXP, assignmentExpression->row, assignmentExpression->column);
            exit(0);
        } else {
            out << lhsType << " " << rhsType << endl;
        }
    } else {
        checkExpression(expression, functionEntry);
    }
}

string checkPostfixExpression(PostfixExpression *postfixExpression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Postfix Expression" << endl;
    if (postfixExpression->flagPostfix == 0) {
        return checkBasicExpression(postfixExpression->basicExpression, functionEntry);
    } else if (postfixExpression->flagPostfix == 1) {
        return checkFunctionCall(postfixExpression->functionCall, functionEntry);
    } else if (postfixExpression->flagPostfix == 2) {
        string postfixIdentifier = string(postfixExpression->postfixIdentifier);
        string scope = postfixExpression->scope;
        string matchFound = checkDeclarationInParentScope(postfixIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            printError(VARIABLE_NOT_DECLARED, postfixExpression->row, postfixExpression->column);
            exit(0);
        } else {
            if (matchFound == "point") {
                if (postfixExpression->memberVariable1 == MemberVariable::x || postfixExpression->memberVariable1 == MemberVariable::y) {
                    return "num";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "line") {
                if (postfixExpression->memberVariable1 == MemberVariable::slope || postfixExpression->memberVariable1 == MemberVariable::c) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "circle") {
                if (postfixExpression->memberVariable1 == MemberVariable::radius || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "ellipse") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "parabola") {
                if (postfixExpression->memberVariable1 == MemberVariable::vertex || postfixExpression->memberVariable1 == MemberVariable::focus) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::latus_rectum || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "hyperbola") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::eccentricity) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    return "point";
                } else if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return "curve";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "curve") {
                if (postfixExpression->memberVariable1 == MemberVariable::a || postfixExpression->memberVariable1 == MemberVariable::b || postfixExpression->memberVariable1 == MemberVariable::c || postfixExpression->memberVariable1 == MemberVariable::f || postfixExpression->memberVariable1 == MemberVariable::g || postfixExpression->memberVariable1 == MemberVariable::h || postfixExpression->memberVariable1 == MemberVariable::delta) {
                    return "num";
                } else if (postfixExpression->memberVariable1 == MemberVariable::equation || postfixExpression->memberVariable1 == MemberVariable::type) {
                    return "string";
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else {
                printError(MEMBER_ACCESS_INVALID, postfixExpression->row, postfixExpression->column);
                exit(0);
            }
        }
    } else if (postfixExpression->flagPostfix == 3) {
        string postfixIdentifier = string(postfixExpression->postfixIdentifier);
        string scope = postfixExpression->scope;
        string matchFound = checkDeclarationInParentScope(postfixIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            printError(VARIABLE_NOT_DECLARED, postfixExpression->row, postfixExpression->column);
            exit(0);
        } else {
            if (matchFound == "line") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2, postfixExpression);
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "circle") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2, postfixExpression);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                        exit(0);
                    }
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "ellipse") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2, postfixExpression);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                        exit(0);
                    }
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "parabola") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2, postfixExpression);
                } else if (postfixExpression->memberVariable1 == MemberVariable::vertex || postfixExpression->memberVariable1 == MemberVariable::focus) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                        exit(0);
                    }
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else if (matchFound == "hyperbola") {
                if (postfixExpression->memberVariable1 == MemberVariable::curve) {
                    return curveMemberAccess(postfixExpression->memberVariable2, postfixExpression);
                } else if (postfixExpression->memberVariable1 == MemberVariable::center) {
                    if (postfixExpression->memberVariable2 == MemberVariable::x || postfixExpression->memberVariable2 == MemberVariable::y) {
                        return "num";
                    } else {
                        printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                        exit(0);
                    }
                } else {
                    printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
                    exit(0);
                }
            } else {
                printError(MEMBER_ACCESS_INVALID, postfixExpression->row, postfixExpression->column);
                exit(0);
            }
        }
    } else if (postfixExpression->flagPostfix == 4) {
        string postfixIdentifier = string(postfixExpression->postfixIdentifier);
        string scope = postfixExpression->scope;
        string matchFound = checkDeclarationInParentScope(postfixIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            printError(VARIABLE_NOT_DECLARED, postfixExpression->row, postfixExpression->column);
            exit(0);
        } else {
            checkExpression(postfixExpression->expression, functionEntry);
            return matchFound;
        }
    }
    else {
        cerr << "\033[1;31m" << "Invalid Postfix Expression" << "\033[0m" << endl;
        exit(0);
    }
}

string curveMemberAccess(MemberVariable memberVariable, PostfixExpression *postfixExpression) {
    if (memberVariable == MemberVariable::a || memberVariable == MemberVariable::b || memberVariable == MemberVariable::c || memberVariable == MemberVariable::f || memberVariable == MemberVariable::g || memberVariable == MemberVariable::h || memberVariable == MemberVariable::delta) {
        return "num";
    } else if (memberVariable == MemberVariable::equation || memberVariable == MemberVariable::type) {
        return "string";
    } else {
        printError(INVALID_MEMBER_ACCESS, postfixExpression->row, postfixExpression->column);
        exit(0);
    }
}

string checkBasicExpression(BasicExpression *basicExpression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Basic Expression" << endl;
    if (basicExpression->flagBasic == 0) {
        return checkConstantValue(basicExpression->constantValue);
    } else if (basicExpression->flagBasic == 1) {
        string basicIdentifier = string(basicExpression->basicIdentifier);
        string scope = basicExpression->scope;
        string matchFound = checkDeclarationInParentScope(basicIdentifier, scope, 1, functionEntry);
        if (matchFound == "") {
            printError(VARIABLE_NOT_DECLARED, basicExpression->row, basicExpression->column);
            exit(0);
        } else {
            return matchFound;
        }
    } else if (basicExpression->flagBasic == 2) {
        return checkExpression(basicExpression->expression, functionEntry);
    } else {
        cerr << "\033[1;31m" << "Invalid Basic Expression" << "\033[0m" << endl;
        exit(0);
    }
}

string checkConstantValue(ConstantValue *constantValue) {
    out << "Checking Constant Value" << endl;
    if (constantValue->flagConstant == 0) {
        return "num";
    } else if (constantValue->flagConstant == 1) {
        return "boolean";
    } else if (constantValue->flagConstant == 2) {
        return "string";
    } else {
        cerr << "\033[1;31m" << "Invalid Constant Value" << "\033[0m" << endl;
        exit(0);
    }
}

string checkFunctionCall(FunctionCall *functionCall, GlobalSymTabEntry *functionEntry) {
    out << "Checking Function Call" << endl;

    string functionCallIdentifier = string(functionCall->functionCallIdentifier);
    vector<Expression *> *argumentList = functionCall->argumentList;

    unordered_set<string> inBuiltFunctions = {"toString", "distance", "solve", "sqrt", "isPoint", "intersection", "tangent", "angle"};

    if (inBuiltFunctions.find(functionCallIdentifier) != inBuiltFunctions.end()) {
        if (functionCallIdentifier == "distance") {
            if(argumentList->size() != 2) {
                printError(INVALID_NO_OF_ARGS_FUNCTION, functionCall->row, functionCall->column);
                exit(0);
            } else {
                string argumentType1 = checkExpression(argumentList->at(0), functionEntry);
                string argumentType2 = checkExpression(argumentList->at(1), functionEntry);
                if (!((argumentType1 == "point" && argumentType2 == "point") || (argumentType1 == "line" && argumentType2 == "line") || (argumentType1 == "line" && argumentType2 == "point"))) {
                    printError(TYPE_MISMATCH_FUNCTION_ARGS, functionCall->row, functionCall->column);
                    exit(0);
                } else {
                    return "num";
                }
            }
        } else if (functionCallIdentifier == "solve") {
            return "point";
        } else if (functionCallIdentifier == "sqrt") {
            return "num";
        } else if (functionCallIdentifier == "isPoint") {
            if(argumentList->size() != 2) {
                printError(INVALID_NO_OF_ARGS_FUNCTION, functionCall->row, functionCall->column);
                exit(0);
            } else {
                string argumentType1 = checkExpression(argumentList->at(0), functionEntry);
                string argumentType2 = checkExpression(argumentList->at(1), functionEntry);
                if (!(argumentType1 == "curve" && argumentType2 == "point")) {
                    printError(TYPE_MISMATCH_FUNCTION_ARGS, functionCall->row, functionCall->column);
                    exit(0);
                } else {
                    return "boolean";
                }
            }
        } else if (functionCallIdentifier == "intersection") {
            return "point";
        } else if (functionCallIdentifier == "tangent") {
            if(argumentList->size() != 2) {
                printError(INVALID_NO_OF_ARGS_FUNCTION, functionCall->row, functionCall->column);
                exit(0);
            } else {
                string argumentType1 = checkExpression(argumentList->at(0), functionEntry);
                string argumentType2 = checkExpression(argumentList->at(1), functionEntry);
                if (!(argumentType1 == "curve" && argumentType2 == "point")) {
                    printError(TYPE_MISMATCH_FUNCTION_ARGS, functionCall->row, functionCall->column);
                    exit(0);
                } else {
                    return "line";
                }
            }
        } else if (functionCallIdentifier == "angle") {
            return "num";
        } else {
            printError(FUNCTION_NOT_DECLARED, functionCall->row, functionCall->column);
        }
    }

    string functionCallKey = functionCallIdentifier;
    GlobalSymTabEntry *entry = searchGlobalSymTab(root->globalSymbolTable, functionCallKey);
    if (entry == NULL) {
        printError(FUNCTION_NOT_DECLARED, functionCall->row, functionCall->column);
        exit(0);
    } else {
        vector<pair<string, string>> *arguments = entry->arguments;
        if (arguments->size() != argumentList->size()) {
            printError(INVALID_NO_OF_ARGS_FUNCTION, functionCall->row, functionCall->column);
            exit(0);
        } else {
            for (int i = 0; i < arguments->size(); i++) {
                string argumentDefaultType = arguments->at(i).first;
                string argumentCallType = checkExpression(argumentList->at(i), functionEntry);
                if (argumentDefaultType != argumentCallType) {
                    printError(TYPE_MISMATCH_FUNCTION_ARGS, functionCall->row, functionCall->column);
                    exit(0);
                    break;
                }
            }
        }
        return entry->dataType;
    }
    return "";
}

string checkExpression(Expression *expression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Expression" << endl;
    if (expression->flagExpression == 0) {
        UnaryExpression *unaryExpression = static_cast<UnaryExpression *>(expression);
        return checkUnaryExpression(unaryExpression, functionEntry);
    } else if (expression->flagExpression == 1) {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        return checkBinaryExpression(binaryExpression->lhs, binaryExpression->rhs, binaryExpression->op, functionEntry);
    } else {
        cerr << "\033[1;31m" << "Invalid Expression" << "\033[0m" << endl;
        exit(0);
    }
}

string checkUnaryExpression(UnaryExpression *unaryExpression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Unary Expression" << endl;
    PostfixExpression *postfixExpression = unaryExpression->postfixExpression;
    vector<UnaryOperator> *opList = unaryExpression->opList;
    string postfixType = checkPostfixExpression(postfixExpression, functionEntry);
    for (auto op : *opList) {
        if (op == UnaryOperator::inc_op or op == UnaryOperator::dec_op) {
            if (postfixType != "num") {
                printError(INVALID_UNARY_OP_ACCESS, unaryExpression->row, unaryExpression->column);
                exit(0);
            }
        } else if (op == UnaryOperator::not_op) {
            if (postfixType != "boolean") {
                printError(INVALID_UNARY_OP_ACCESS, unaryExpression->row, unaryExpression->column);
                exit(0);
            }
        } else if (op == UnaryOperator::minus_op || op == UnaryOperator::plus_op) {
            if (postfixType != "num") {
                printError(INVALID_UNARY_OP_ACCESS, unaryExpression->row, unaryExpression->column);
                exit(0);
            }
        } else {
            printError(INVALID_UNARY_OP_ACCESS, unaryExpression->row, unaryExpression->column);
            exit(0);
        }
    }
    return postfixType;
}

string checkBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry) {
    out << "Checking Binary Expression" << endl;
    string lhsType = checkExpression(lhs, functionEntry);
    string rhsType = checkExpression(rhs, functionEntry);
    if (lhsType != rhsType) {
        printError(TYPE_MISMATCH_BINARYEXP, lhs->row, lhs->column);
        exit(0);
    } else if(lhsType == "num" && rhsType == "num") {
        return "num";
    } else if(lhsType == "boolean" && rhsType == "boolean" && (op != BinaryOperator::div_op || op != BinaryOperator::mod_op)) {
        return "boolean";
    } else if(lhsType == "point" && rhsType == "point" && (op == BinaryOperator::add_op || op == BinaryOperator::sub_op)) {
        return "point";
    } else {
        return "";
    }
}

void checkCompoundStatement(CompoundStatement *compoundStatement, GlobalSymTabEntry *functionEntry) {
    out << "Checking Compound Statement" << endl;
    vector<Statement *> *statementList = compoundStatement->statementList;
    for (auto statementItem : *statementList) {
        if (statementItem->flagStatement == 0) {
            out << "AssignmentExpression inside function " << functionEntry->name << endl;
            checkAssignmentExpression(statementItem->assignmentExpression, functionEntry);
        } else if (statementItem->flagStatement == 1) {
            out << "Declaration inside function " << functionEntry->name << endl;
            checkDeclaration(statementItem->declaration, functionEntry);
        } else if (statementItem->flagStatement == 2) {
            out << "InOutStatement inside function " << functionEntry->name << endl;
            inOutStatement(statementItem->inOut, functionEntry);
        } else if (statementItem->flagStatement == 3) {
            out << "CompoundStatement inside function " << functionEntry->name << endl;
            checkCompoundStatement(statementItem->compoundStatement, functionEntry);
        } else if (statementItem->flagStatement == 4) {
            out << "ConditionalStatement inside function " << functionEntry->name << endl;
            checkConditionalStatement(statementItem->conditionalStatement, functionEntry);
        } else if (statementItem->flagStatement == 5) {
            out << "IterativeStatement inside function " << functionEntry->name << endl;
            checkIterativeStatement(statementItem->iterativeStatement, functionEntry);
        } else if (statementItem->flagStatement == 6) {
            out << "JumpStatement inside function " << functionEntry->name << endl;
            checkJumpStatement(statementItem->jumpStatement, functionEntry);
        }
    }
}

void checkConditionalStatement(ConditionalStatement *conditionalStatement, GlobalSymTabEntry *functionEntry) {
    out << "Checking Conditional Statement" << endl;
    Expression *expression = conditionalStatement->expression;
    CompoundStatement *compoundStatement1 = conditionalStatement->compoundStatement1;
    vector<ElseIf *> *elseIfList = conditionalStatement->elseIfList;
    CompoundStatement *compoundStatement2 = conditionalStatement->compoundStatement2;

    string expressionType = checkConditionalExpression(expression, functionEntry);
    if (expressionType != "boolean") {
        printError(INVALID_CONDITIONAL_EXP, conditionalStatement->row, conditionalStatement->column);
        exit(0);
    }
    checkCompoundStatement(compoundStatement1, functionEntry);

    for (auto elseIfItem : *elseIfList) {
        Expression *expression = elseIfItem->expression;
        CompoundStatement *compoundStatement = elseIfItem->compoundStatement;
        string expressionType = checkConditionalExpression(expression, functionEntry);
        if (expressionType != "boolean") {
            printError(INVALID_CONDITIONAL_EXP, elseIfItem->row, elseIfItem->column);
            exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    }

    if (compoundStatement2 != NULL) {
        checkCompoundStatement(compoundStatement2, functionEntry);
    }
}

void checkIterativeStatement(IterativeStatement *iterativeStatement, GlobalSymTabEntry *functionEntry) {
    out << "Checking Iterative Statement" << endl;
    if (iterativeStatement->isWhile == 0) {
        Expression *expression2 = iterativeStatement->expression2;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        string expressionType = checkConditionalExpression(expression2, functionEntry);
        if (expressionType != "boolean") {
            printError(INVALID_LOOP_EXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
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
            printError(INVALID_LOOP_EXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
        }
        string assignmentType = checkIterativeAssignmentExpression(expression3, functionEntry);
        if (assignmentType != "Initialisation") {
            printError(TYPE_MISMATCH_ASSIGNEXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    } else if (iterativeStatement->isWhile == 2) {
        AssignmentExpression *expression1 = iterativeStatement->assignmentexpression1;
        Expression *expression2 = iterativeStatement->expression2;
        AssignmentExpression *expression3 = iterativeStatement->assignmentexpression3;
        CompoundStatement *compoundStatement = iterativeStatement->compoundStatement;

        string assignmentType1 = checkIterativeAssignmentExpression(expression1, functionEntry);
        if (assignmentType1 != "Initialisation") {
            printError(TYPE_MISMATCH_ASSIGNEXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
        }
        string expressionType = checkConditionalExpression(expression2, functionEntry);
        if (expressionType != "boolean") {
            printError(INVALID_LOOP_EXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
        }
        string assignmentType2 = checkIterativeAssignmentExpression(expression3, functionEntry);
        if (assignmentType2 != "Initialisation") {
            printError(TYPE_MISMATCH_ASSIGNEXP, iterativeStatement->row, iterativeStatement->column);
            exit(0);
        }
        checkCompoundStatement(compoundStatement, functionEntry);
    }
}

void checkJumpStatement(JumpStatement *jumpStatement, GlobalSymTabEntry *functionEntry) {
    out << "Checking Jump Statement" << endl;
    if (jumpStatement->flagJump == 3) {
        Expression *expression = jumpStatement->expression;
        string expressionType = checkExpression(expression, functionEntry);
        if (expressionType != functionEntry->dataType) {
            printError(INVALID_RETURN_TYPE, jumpStatement->row, jumpStatement->column);
            exit(0);
        }
    }
}

string checkConditionalExpression(Expression *expression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Conditional Expression" << endl;
    if (expression->flagExpression == 1) {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        return checkConditionalBinaryExpression(binaryExpression->lhs, binaryExpression->rhs, binaryExpression->op, functionEntry);
    } else {
        printError(INVALID_CONDITIONAL_EXP, expression->row, expression->column);
        exit(0);
    }
}

string checkConditionalBinaryExpression(Expression *lhs, Expression *rhs, BinaryOperator op, GlobalSymTabEntry *functionEntry) {
    out << "Checking Conditional Binary Expression" << endl;
    string lhsType = checkExpression(lhs, functionEntry);
    string rhsType = checkExpression(rhs, functionEntry);
    if (lhsType != rhsType) {
        printError(TYPE_MISMATCH_BINARYEXP, lhs->row, lhs->column);
        exit(0);
    } else {
        if (op == BinaryOperator::equal_op or op == BinaryOperator::not_equal_op or op == BinaryOperator::less_equal_op or op == BinaryOperator::less_op or op == BinaryOperator::greater_equal_op or op == BinaryOperator::greater_op or op == BinaryOperator::and_op or op == BinaryOperator::or_op) {
            return "boolean";
        } else {
            printError(INVALID_CONDITIONAL_EXP, lhs->row, lhs->column);
            exit(0);
        }
    }
    return "";
}

string checkIterativeAssignmentExpression(AssignmentExpression *assignmentExpression, GlobalSymTabEntry *functionEntry) {
    out << "Checking Iterative Assignment Expression" << endl;
    PostfixExpression *postfixExpression = assignmentExpression->postfixExpression;
    Expression *expression = assignmentExpression->expression;
    if (postfixExpression != NULL) {
        string lhsType = checkPostfixExpression(postfixExpression, functionEntry);
        string rhsType = checkExpression(expression, functionEntry);
        if (lhsType != rhsType) {
            printError(TYPE_MISMATCH_ASSIGNEXP, assignmentExpression->row, assignmentExpression->column);
            exit(0);
        } else {
            return "Initialisation";
        }
    } else {
        cerr << "\033[1;31m" << "Invalid Iterative Assignment Expression" << "\033[0m" << endl;
        exit(0);
    }
}