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
