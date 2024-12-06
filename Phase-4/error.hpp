#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

#define INVALID_HEADER 0
#define INVALID_MACRO 1
#define VARIABLE_NOT_DECLARED 2
#define VARIABLE_REDECLARATION 3
#define VARIABLE_REDECLARATION_FUNC_ARGUMENT 4
#define FUNCTION_NOT_DECLARED 5
#define FUNCTION_REDECLARATION 6
#define INVALID_NO_OF_ARGS_DECLARATION 7
#define INVALID_NO_OF_ARGS_FUNCTION 8
#define TYPE_MISMATCH_DECLARATION 9
#define TYPE_MISMATCH_FUNCTION 10
#define TYPE_MISMATCH_ASSIGNEXP 11
#define INVALID_MEMBER_ACCESS 12
#define MEMBER_ACCESS_INVALID 13
#define INVALID_UNARY_OP_ACCESS 14
#define TYPE_MISMATCH_BINARYEXP 15
#define INVALID_CONDITIONAL_EXP 16
#define INVALID_LOOP_EXP 17
#define INVALID_RETURN_TYPE 18
#define INVALID_DECLARATION 19

void printError(int errorType, int yylineno, int yycolno);

#endif
