%{
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin, *tokfile, *parsefile ;
extern int yylineno;
int yylex();
int yyerror(char *s);
%}

%token VOID STRING BOOL
%token IF ELIF ELSE LOOP STOP SKIP RETURN SWITCH GOTO WRITE READ STRUCT INCLUDE DEFINE NULL_TOK MAIN DEFAULT CASE TRUE FALSE
%token NUM POINT EQUATION LINE CIRCLE PARABOLA HYPERBOLA ELLIPSE CURVE
%token TO_STRING DISTANCE SOLVE SQRT ISPOINT INTERSECTION TANGENT SLOPE_LINE ANGLE TYPE 
%token X Y SLOPE EQUATION_MV CENTER RADIUS A B C F G H DELTA ECCENTRICITY LATUS_RECTUM
%token IDENTIFIER
%token COLON SEMICOLON COMMA SINGLE_QUOTE DOUBLE_QUOTE LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE LESS_THAN GREATER_THAN DOLLAR
%token INCREMENT DECREMENT LOGICAL_NOT EXPONENT MULTIPLY DIVIDE MODULUS PLUS MINUS EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL LOGICAL_AND LOGICAL_OR ASSIGN TILDE MEMBER_ACCESS
%token NEW_LINE_CHARACTER TAB_CHARACTER
%token STRING_LITERAL CONSTANT

%%
program
    : program expression SEMICOLON {printf("Expression\n");}
    | program declaration {printf("Declaration\n");}
    | program statements {printf("Statement\n");}
    | program function_declaration {printf("Function Declaration\n");}
    | program headers {printf("Headers\n");}
    | program in_out {printf("Read Write\n");}
    |
    ;

headers
    : DOLLAR INCLUDE STRING_LITERAL SEMICOLON
    | DOLLAR DEFINE IDENTIFIER CONSTANT SEMICOLON
    ;

in_out
    : WRITE TILDE print SEMICOLON
    | READ TILDE scan SEMICOLON
    ;

scan
    : IDENTIFIER
    | IDENTIFIER TILDE scan
    ;

print
    : NEW_LINE_CHARACTER 
    | TAB_CHARACTER
    | postfix_expression
    | NEW_LINE_CHARACTER TILDE print
    | TAB_CHARACTER TILDE print
    | postfix_expression TILDE print
    ;

member_variable
    : X
    | Y
    | SLOPE
    | EQUATION_MV
    | CENTER
    | RADIUS
    | A
    | B
    | C
    | F
    | G
    | H
    | DELTA
    | ECCENTRICITY
    | LATUS_RECTUM
    ;


%%

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <input file> <token file> <parse file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    tokfile = fopen(argv[2], "w");
    if (tokfile == NULL) {
        printf("Error: Cannot open token file %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    parsefile = fopen(argv[3], "w");
    if (parsefile == NULL) {
        printf("Error: Cannot open parse file %s\n", argv[3]);
        fclose(inputFile);
        fclose(tokfile);
        return 1;
    }

    yyin = inputFile;

    int i = yyparse();
    if(i == 0){
        printf("Parsing done successfully\n");
    }
    else{
        printf("Parsing failed\n");
    }

    // int token = 0;
    // while ((token = yylex()) != 0) {
    //     // printf("Token: %d\n", token);
    // }

    fclose(inputFile);
    fclose(tokfile);
    fclose(parsefile);

    return 0;
}
