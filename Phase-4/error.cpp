#include "error.hpp"
#include <iostream>
#include <string>
using namespace std;

void printError(int errorType, int yylineno, int yycolno) {
    string error = "";
    if (errorType == INVALID_HEADER) {
        error = "Invalid header file extension at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_MACRO) {
        error = "Invalid macro definition at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == VARIABLE_NOT_DECLARED) {
        error = "Variable not declared at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == VARIABLE_REDECLARATION) {
        error = "Variable redeclaration error at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == VARIABLE_REDECLARATION_FUNC_ARGUMENT) {
        error = "Variable redeclaration error in function argument at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == FUNCTION_NOT_DECLARED) {
        error = "Function not declared at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == FUNCTION_REDECLARATION) {
        error = "Function redeclaration error at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_NO_OF_ARGS_DECLARATION) {
        error = "Invalid number of arguments in declaration at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_NO_OF_ARGS_FUNCTION) {
        error = "Invalid number of arguments in function call at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == TYPE_MISMATCH_DECLARATION) {
        error = "Type mismatch in declaration at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == TYPE_MISMATCH_FUNCTION) {
        error = "Type mismatch in return type of function call at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == TYPE_MISMATCH_FUNCTION_ARGS) {
        error = "Type mismatch in function arguments at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == TYPE_MISMATCH_ASSIGNEXP) {
        error = "Type mismatch in assignment expression at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_MEMBER_ACCESS) {
        error = "Invalid member access at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == MEMBER_ACCESS_INVALID) {
        error = "Member access invalid at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_UNARY_OP_ACCESS) {
        error = "Invalid unary operator access at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == TYPE_MISMATCH_BINARYEXP) {
        error = "Type mismatch in binary operator at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_CONDITIONAL_EXP) {
        error = "Invalid conditional expression at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_LOOP_EXP) {
        error = "Invalid loop expression at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_RETURN_TYPE) {
        error = "Invalid return type at line " + to_string(yylineno) + " column " + to_string(yycolno);
    } else if (errorType == INVALID_DECLARATION) {
        error = "Invalid declaration at line " + to_string(yylineno) + " column " + to_string(yycolno);
    }

    cerr << "\033[1;31m" << error << "\033[0m" << endl;
    cerr << "\033[1;31m" << "SEMANTIC VERDICT: [FAILED]" << "\033[0m" << endl;
}
