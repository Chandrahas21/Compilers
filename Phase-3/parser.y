%{
#include <stdio.h>
#include <stdlib.h>
int yywrap();
int yylex();          
int yyerror(char* s); 
%}

%token COMMENT_SINGLE_LINE COMMENT_MULTI_LINE
%token VOID STRING BOOL
%token IF ELIF ELSE LOOP STOP SKIP RETURN SWITCH GOTO WRITE READ STRUCT INCLUDE DEFINE NULL_TOK MAIN DEFAULT CASE TRUE FALSE
%token NUM POINT EQUATION LINE CIRCLE PARABOLA HYPERBOLA ELLIPSE CURVE
%token TO_STRING DISTANCE SOLVE SQRT ISPOINT INTERSECTION TANGENT SLOPE_LINE ANGLE TYPE 
%token X Y SLOPE EQUATION_MV CENTER RADIUS A B C F G H DELTA ECCENTRICITY LATUS_RECTUM
%token IDENTIFIER
%token COLON SEMICOLON COMMA SINGLE_QUOTE DOUBLE_QUOTE LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE LESS_THAN GREATER_THAN DOLLAR
%token INCREMENT DECREMENT LOGICAL_NOT EXPONENT MULTIPLY DIVIDE MODULUS PLUS MINUS EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL LOGICAL_AND LOGICAL_OR ASSIGN TILDE MEMBER_ACCESS
%token NEW_LINE_CHARACTER TAB_CHARACTER
%token INTEGER
%token FLOATING_POINT_NUMBER
%token STRING_LITERAL

%start first

%%
first : PLUS MINUS SEMICOLON {printf("Valid\n"); }
      ;
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