%{
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin, *tokenFile, *outputFile ;
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

%right ASSIGN
%left LOGICAL_AND LOGICAL_OR
%left EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL GREATER_THAN LESS_THAN
%left PLUS MINUS
%left EXPONENT
%left MULTIPLY DIVIDE MODULUS

%%
program
    : program all_tokens
    | 
    ;

all_tokens
    : expression SEMICOLON
    | declaration
    | statements
    | function_declaration
    | headers
    | in_out
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

basic_expression
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | TRUE
    | FALSE
    // | expression
    | LEFT_PAREN expression RIGHT_PAREN 
    ;

postfix_expression
    : basic_expression
    | postfix_expression LEFT_PAREN RIGHT_PAREN
	| postfix_expression LEFT_PAREN declaration_arg_list RIGHT_PAREN
    | default_function_list LEFT_PAREN RIGHT_PAREN
    | default_function_list LEFT_PAREN declaration_arg_list RIGHT_PAREN 
    | postfix_expression INCREMENT
    | postfix_expression DECREMENT
    | postfix_expression MEMBER_ACCESS member_variable
    ;

unary_operator
    : LOGICAL_NOT
    | PLUS
    | MINUS
    ;

unary_expression
    : postfix_expression
    | INCREMENT unary_expression
    | DECREMENT unary_expression
    | unary_operator unary_expression
    ;

/* left_identifier
    : IDENTIFIER
    | IDENTIFIER MEMBER_ACCESS member_variable
    ; */

expression
    : postfix_expression ASSIGN evalute_expression
    | evalute_expression
    ;

evalute_expression
    : unary_expression
    | evalute_expression MULTIPLY evalute_expression
    | evalute_expression DIVIDE evalute_expression
    | evalute_expression MODULUS evalute_expression
    | evalute_expression EXPONENT evalute_expression
    | evalute_expression PLUS evalute_expression
    | evalute_expression MINUS evalute_expression
    | evalute_expression EQUAL evalute_expression
    | evalute_expression NOT_EQUAL evalute_expression
    | evalute_expression GREATER_EQUAL evalute_expression
    | evalute_expression LESS_EQUAL evalute_expression
    | evalute_expression GREATER_THAN evalute_expression
    | evalute_expression LESS_THAN evalute_expression
    | evalute_expression LOGICAL_AND evalute_expression
    | evalute_expression LOGICAL_OR evalute_expression
   // | LEFT_PAREN evalute_expression RIGHT_PAREN
    ;

declaration_type
    : VOID
    | STRING
    | BOOL
    | NUM
    | POINT 
    | EQUATION
    | LINE
    | CIRCLE
    | PARABOLA
    | HYPERBOLA
    | ELLIPSE
    | CURVE
    ;

declaration_arg_list
    : expression 
    | expression COMMA declaration_arg_list
    ;

declaration_set_index
    : IDENTIFIER
    | IDENTIFIER ASSIGN evalute_expression
    | IDENTIFIER LEFT_PAREN declaration_arg_list RIGHT_PAREN
    ;

declaration_set
    : declaration_set_index
    | declaration_set_index COMMA declaration_set
    ;

declaration
    : declaration_type SEMICOLON // int ; check later
    | declaration_type declaration_set SEMICOLON 
    ;

function_arg_list
    : declaration_type IDENTIFIER
    | declaration_type IDENTIFIER COMMA function_arg_list
    ;

default_function_list
    : TO_STRING
    | DISTANCE
    | SOLVE
    | SQRT
    | ISPOINT
    | INTERSECTION
    | TANGENT
    | SLOPE_LINE
    | ANGLE
    | TYPE
    | MAIN
    ;

function_declaration
    : LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN RIGHT_PAREN
    | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN RIGHT_PAREN
    | LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN function_arg_list RIGHT_PAREN
    | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN function_arg_list RIGHT_PAREN 
    // | LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN RIGHT_PAREN compound_statement
    // | LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN function_arg_list RIGHT_PAREN compound_statement
    // | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN RIGHT_PAREN compound_statement
    // | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN function_arg_list RIGHT_PAREN compound_statement
    ;

statements
    : selective_statement {printf("Selective Statments\n");}
    | jump_statements {printf("Jump Statments\n");}
    | compound_statement {printf("Compound Statments\n");}
    | label_statement {printf("Label Statments\n");}
    | iterative_statement {printf("Iterative Statments\n");}
    ;

compound_statement
    : LEFT_BRACE program RIGHT_BRACE
    ;

selective_statement
    : SWITCH LEFT_PAREN expression RIGHT_PAREN compound_statement
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement ELSE compound_statement
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement else_if
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement else_if ELSE compound_statement
    ;

else_if
    : ELIF LEFT_PAREN expression RIGHT_PAREN compound_statement
    | ELIF LEFT_PAREN expression RIGHT_PAREN compound_statement else_if
    ;  

constant_expression
    : CONSTANT 
    | STRING_LITERAL
    ;

label_statement
    : IDENTIFIER COLON compound_statement 
    | CASE constant_expression COLON compound_statement
    | DEFAULT COLON compound_statement
    ;

iterative_statement
    : LOOP LEFT_PAREN expression RIGHT_PAREN compound_statement
    | LOOP LEFT_PAREN expression SEMICOLON expression SEMICOLON expression RIGHT_PAREN compound_statement 
    | LOOP LEFT_PAREN declaration expression SEMICOLON expression RIGHT_PAREN compound_statement 
    ;

jump_statements
    : STOP SEMICOLON 
    | SKIP SEMICOLON 
    | RETURN SEMICOLON
    | RETURN expression SEMICOLON
    | GOTO IDENTIFIER SEMICOLON
    ;
%%

int yyerror(char *s) {
    fprintf(outputFile, "\nError: %s at line %d.\n", s, yylineno);
    fprintf(tokenFile, "\nError: %s at line %d.\n", s, yylineno);
    printf("Error: %s at line %d.\n", s, yylineno);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <input file> <output file> <token file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        fclose(inputFile);
        return 1;
    }

    outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        printf("Error: Cannot open parse file %s\n", argv[2]);
        fclose(outputFile);
        return 1;
    }

    tokenFile = fopen(argv[3], "w");
    if (tokenFile == NULL) {
        printf("Error: Cannot open token file %s\n", argv[3]);
        fclose(tokenFile);
        return 1;
    }

    yyin = inputFile;

    int i = yyparse();
    if(i == 0){
        printf("Parsing done successfully\n");
    }
    else{
        /* printf("Parsing failed at line no %d\n", yylineno); */
    }

    // int token = 0;
    // while ((token = yylex()) != 0) {
    //     // printf("Token: %d\n", token);
    // }

    fclose(inputFile);
    fclose(outputFile);
    fclose(tokenFile);

    return 0;
}