#include "symbolTable.hpp"
#include <iomanip>
using namespace std;

void insertGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *globalSymbolTable, string id, string dataType, string isGlobal, string scope, vector<pair<string, string>> *arguments, int yylineno, int yycolumnno) {
    string key = id;
    GlobalSymTabEntry *entry = searchGlobalSymTab(globalSymbolTable, key);
    if (entry == NULL) {
        auto &ref = (*globalSymbolTable);
        ref[key].name = id;
        ref[key].dataType = dataType;
        ref[key].isGlobal = isGlobal;
        ref[key].scope = scope;
        ref[key].arguments = arguments;
        ref[key].rowNum = yylineno;
        ref[key].colNum = yycolumnno;
        if (isGlobal == "Function") {
            ref[key].symbolTable = new unordered_map<string, SymTabEntry>;
        } else {
            ref[key].symbolTable = nullptr;
        }
    } else {
        cerr << "\033[1;31m" << "Re-Declaration of " << "\"" << id << "\"" << " at line no " << yylineno << ".\033[0m" << endl;
        // exit(1);
    }
}

void insertLocalSymTab(unordered_map<string, SymTabEntry> *symbolTable, string id, string dataType, string scope, int yylineno, int yycolumnno) {
    string key = id + " <=> " + scope;
    SymTabEntry *entry = searchLocalSymTab(symbolTable, key);
    if (entry == NULL) {
        auto &ref = (*symbolTable);
        ref[key].name = id;
        ref[key].dataType = dataType;
        ref[key].scope = scope;
        ref[key].rowNum = yylineno;
        ref[key].colNum = yycolumnno;
    } else {
        cerr << "\033[1;31m" << "Re-Declaration of " << "\"" << id << "\"" << " at line no " << yylineno << ".\033[0m" << endl;
        // exit(1);
    }
}

GlobalSymTabEntry *searchGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *globalSymbolTable, string key) {
    auto it = globalSymbolTable->find(key);
    if (it == globalSymbolTable->end()) {
        return NULL;
    }
    return &(it->second);
}

SymTabEntry *searchLocalSymTab(unordered_map<string, SymTabEntry> *symbolTable, string key) {
    auto it = symbolTable->find(key);
    if (it == symbolTable->end()) {
        return NULL;
    }
    return &(it->second);
}

void printGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *globalSymbolTable) {
    cout << "\033[1;32m" << "Global Symbol Table" << "\033[0m" << endl;
    cout << setw(20) << "Key"
         << setw(20) << "Name"
         << setw(20) << "Data Type"
         << setw(20) << "Type"
         << setw(20) << "Scope"
         << setw(20) << "Row Number"
         << setw(20) << "Column Number" << endl;
    for (auto it = globalSymbolTable->begin(); it != globalSymbolTable->end(); it++) {
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << setw(20)
             << it->first << setw(20)
             << it->second.name << setw(20)
             << it->second.dataType << setw(20)
             << it->second.isGlobal << setw(20)
             << it->second.scope << setw(20)
             << it->second.rowNum << setw(20)
             << it->second.colNum << endl;
        if (it->second.symbolTable != nullptr) {
            cout << "\033[1;32m" << "Local Symbol Table" << "\033[0m" << endl;
            printLocalSymTab(it->second.symbolTable);
            cout << endl;
        }
    }
}

void printLocalSymTab(unordered_map<string, SymTabEntry> *symbolTable) {
    cout << setw(20) << "Name"
         << setw(20) << "Data Type"
         << setw(20) << "Scope"
         << setw(20) << "Row Number"
         << setw(20) << "Column Number" << endl;
    for (auto it = symbolTable->begin(); it != symbolTable->end(); it++) {
        cout << setw(20) << it->second.name
             << setw(20) << it->second.dataType
             << setw(20) << it->second.scope
             << setw(20) << it->second.rowNum
             << setw(20) << it->second.colNum << endl;
    }
}