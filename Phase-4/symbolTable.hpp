#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class SymTabEntry {
public:
    string name;
    string dataType;
    string scope;
    int rowNum;
    int colNum;
};

class GlobalSymTabEntry {
public:
    string name;
    string dataType;
    string isGlobal; // function or global variable
    string scope;
    vector<pair<string, string>> *arguments; // for global variables, it will be NULL
    int rowNum;
    int colNum;
    unordered_map<string, SymTabEntry> *symbolTable; // for global variables, it will be NULL
};

void insertGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *, string, string, string, string, int, int);
void insertGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *, string, string, string, string, vector<pair<string, string>> *, int, int);
void insertLocalSymTab(unordered_map<string, SymTabEntry> *, string, string, string, int, int);
GlobalSymTabEntry *searchGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *, string);
SymTabEntry *searchLocalSymTab(unordered_map<string, SymTabEntry> *, string);
void printGlobalSymTab(unordered_map<string, GlobalSymTabEntry> *);
void printLocalSymTab(unordered_map<string, SymTabEntry> *);
void error(unordered_map<string, SymTabEntry> *, string);