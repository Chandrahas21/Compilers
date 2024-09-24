%{
#include <stdio.h>
#include <stdlib.h>
int yywrap();
int yylex();          
int yyerror(char* s); 
%}

%token COMMENT_SINGLE_LINE
%token COMMENT_MULTI_LINE
%token VOID
%token STRING
%token BOOL
%token IF
%token ELIF
%token ELSE
%token LOOP
%token STOP
%token SKIP
%token RETURN
%token SWITCH
%token GOTO
%token WRITE
%token READ
%token STRUCT
%token INCLUDE
%token DEFINE
%token NULL_TOK
%token MAIN
%token DEFAULT
%token CASE
%token TRUE
%token FALSE
%token NUM
%token POINT
%token EQUATION
%token LINE
%token CIRCLE
%token PARABOLA
%token HYPERBOLA
%token ELLIPSE
%token CURVE
%token TO_STRING
%token DISTANCE
%token SOLVE
%token SQRT
%token ISPOINT
%token INTERSECTION
%token TANGENT
%token SLOPE_LINE
%token ANGLE
%token TYPE
%token X
%token Y
%token SLOPE
%token EQUATION_MV
%token CENTER
%token RADIUS
%token A
%token B
%token C
%token F
%token G
%token H
%token DELTA
%token ECCENTRICITY
%token LATUS_RECTUM
%token IDENTIFIER
%token COLON
%token SEMICOLON
%token COMMA
%token SINGLE_QUOTE
%token DOUBLE_QUOTE
%token LEFT_PAREN
%token RIGHT_PAREN
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_BRACE
%token RIGHT_BRACE
%token LESS_THAN
%token GREATER_THAN
%token DOLLAR
%token INCREMENT
%token DECREMENT
%token LOGICAL_NOT
%token EXPONENT
%token MULTIPLY
%token DIVIDE
%token MODULUS
%token PLUS
%token MINUS
%token EQUAL
%token NOT_EQUAL
%token GREATER_EQUAL
%token LESS_EQUAL
%token LOGICAL_AND
%token LOGICAL_OR
%token ASSIGN
%token TILDE
%token MEMBER_ACCESS
%token NEW_LINE_CHARACTER
%token TAB_CHARACTER
%token INTEGER
%token FLOATING_POINT_NUMBER
%token STRING_LITERAL

%%

%%

int yywrap(void) {
    return 1;
}

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

int main(void) {
    yyparse();
    return 0;
}