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
