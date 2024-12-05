%{
    #include <iostream>
    #include <string>
    #include <fstream>
    using namespace std;

    int yylex();
    int yyparse();
    int yyerror(string s);
    extern int yylineno;
    extern FILE *yyin;
    extern ofstream tokenFile, outputFile;

    #include "symbolTable.hpp"
    #include <unordered_map>

    unordered_map<string, SymbolTableEntry> *symbolTable = new unordered_map<string, SymbolTableEntry>();
    string dataType = "";
    string type = "";

    #include "scopeDepth.hpp"
    vector<int> scopeDepthVector = {};
    vector<string> allscopes = {};
    string scope = "";
%}

%union {
    float float_val;
    char* str_val;
}

%token <str_val> VOID STRING BOOL
%token <str_val> IF ELIF ELSE LOOP STOP SKIP RETURN SWITCH GOTO WRITE READ STRUCT INCLUDE DEFINE NULL_TOK MAIN DEFAULT CASE TRUE FALSE
%token <str_val> NUM POINT EQUATION LINE CIRCLE PARABOLA HYPERBOLA ELLIPSE CURVE
%token <str_val> TO_STRING DISTANCE SOLVE SQRT ISPOINT INTERSECTION TANGENT SLOPE_LINE ANGLE TYPE 
%token <str_val> X Y SLOPE EQUATION_MV RADIUS A B C F G H DELTA ECCENTRICITY LATUS_RECTUM
%token <str_val> VERTEX CENTER FOCUS    
%token <str_val> IDENTIFIER
%token <str_val> COLON SEMICOLON COMMA SINGLE_QUOTE DOUBLE_QUOTE LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE LESS_THAN GREATER_THAN DOLLAR
%token <str_val> INCREMENT DECREMENT LOGICAL_NOT EXPONENT MULTIPLY DIVIDE MODULUS PLUS MINUS EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL LOGICAL_AND LOGICAL_OR ASSIGN TILDE MEMBER_ACCESS
%token <str_val> NEW_LINE_CHARACTER TAB_CHARACTER
%token <str_val> STRING_LITERAL 
%token <float_val> CONSTANT

%right ASSIGN   
%left LOGICAL_AND LOGICAL_OR
%left EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL GREATER_THAN LESS_THAN
%left PLUS MINUS
%left EXPONENT
%left MULTIPLY DIVIDE MODULUS

%%
start
    : headers program
    ;

headers
    : macros
    | macros headers 
    ;

macros
    : DOLLAR INCLUDE STRING_LITERAL SEMICOLON
    | DOLLAR DEFINE IDENTIFIER CONSTANT SEMICOLON
    ;

program
    : all_tokens
    | program all_tokens
    ;

all_tokens
    : declaration SEMICOLON
    | function_declaration
    ;

statement
    : assignment_expression SEMICOLON
    | declaration SEMICOLON
    | statements
    | in_out
    ;

statement_list
    : statement
    | statement statement_list
    ;

in_out
    : WRITE TILDE print SEMICOLON
    | READ TILDE scan SEMICOLON
    ;

scan
    : IDENTIFIER
    | IDENTIFIER TILDE scan
    ;

printset
    : NEW_LINE_CHARACTER
    | TAB_CHARACTER
    | postfix_expression
    ;

print
    : printset
    | printset TILDE print
    ;

member_variable
    : X
    | Y
    | SLOPE
    | EQUATION_MV
    | CENTER
    | FOCUS
    | VERTEX
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
    | TRUE
    | FALSE
    | STRING_LITERAL
    | LEFT_PAREN expression RIGHT_PAREN 
    ;

function_call
    : IDENTIFIER LEFT_PAREN RIGHT_PAREN
	| IDENTIFIER LEFT_PAREN declaration_arg_list RIGHT_PAREN
    | default_function_list LEFT_PAREN RIGHT_PAREN
    | default_function_list LEFT_PAREN declaration_arg_list RIGHT_PAREN 
    ;

postfix_expression
    : basic_expression
    | function_call
    | IDENTIFIER MEMBER_ACCESS member_variable
    | IDENTIFIER MEMBER_ACCESS member_variable MEMBER_ACCESS member_variable
    | postfix_expression INCREMENT
    | postfix_expression DECREMENT
    ;

unary_operator
    : LOGICAL_NOT
    | PLUS
    | MINUS
    | INCREMENT
    | DECREMENT
    ;
    
unary_expression
    : postfix_expression 
    | unary_operator unary_expression
    ;

assignment_expression
    : postfix_expression ASSIGN expression
    | expression
    ;

expression
    : unary_expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | expression MODULUS expression
    | expression EXPONENT expression
    | expression PLUS expression
    | expression MINUS expression
    | expression EQUAL expression
    | expression NOT_EQUAL expression
    | expression GREATER_EQUAL expression
    | expression LESS_EQUAL expression
    | expression GREATER_THAN expression
    | expression LESS_THAN expression
    | expression LOGICAL_AND expression
    | expression LOGICAL_OR expression
    ;

declaration_type
    : VOID { dataType = "Void"; }
    | STRING { dataType = "String"; }
    | BOOL { dataType = "Bool"; }
    | NUM { dataType = "Num"; }
    | POINT { dataType = "Point"; }
    | EQUATION { dataType = "Equation"; }
    | LINE { dataType = "Line"; }
    | CIRCLE { dataType = "Circle"; }
    | PARABOLA { dataType = "Parabola"; }
    | HYPERBOLA { dataType = "Hyperbola"; }
    | ELLIPSE { dataType = "Ellipse"; }
    | CURVE { dataType = "Curve"; }
    ;

declaration_arg_list
    : expression 
    | expression COMMA declaration_arg_list
    ;

declaration_set_index
    : IDENTIFIER {
        string id = string($1);
        insert(symbolTable, id, dataType, "Variable", scope, yylineno);
    }
    | IDENTIFIER ASSIGN expression {
        string id = string($1);
        insert(symbolTable, id, dataType, "Variable", scope, yylineno);
    }
    | IDENTIFIER LEFT_PAREN declaration_arg_list RIGHT_PAREN {
        string id = string($1);
        insert(symbolTable, id, dataType, "Variable", scope, yylineno);
    }
    ;

declaration_set
    : declaration_set_index 
    | declaration_set_index COMMA declaration_set
    ;

declaration
    : declaration_type declaration_set 
    ;

function_arg_list
    : declaration_type IDENTIFIER {
        string id = string($2);
        insert(symbolTable, id, dataType, "Function Arg", scope, yylineno);
    }
    | declaration_type IDENTIFIER COMMA function_arg_list {
        string id = string($2);
        insert(symbolTable, id, dataType, "Function Arg", scope, yylineno);
    }
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
    : LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN RIGHT_PAREN compound_statement
    | LESS_THAN declaration_type GREATER_THAN default_function_list LEFT_PAREN function_arg_list RIGHT_PAREN compound_statement
    | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN RIGHT_PAREN compound_statement
    | LESS_THAN declaration_type GREATER_THAN IDENTIFIER LEFT_PAREN function_arg_list RIGHT_PAREN compound_statement
    ;

statements
    : compound_statement
    | selective_statement
    | iterative_statement
    | jump_statement
    ;

openbrace_statement
    : LEFT_BRACE { 
        if (scopeDepthVector.empty())
        {
            scopeDepthVector.push_back(1);
        }
        else
        {
            ++scopeDepthVector.back();
        }
        allscopes.push_back(printDepth(scopeDepthVector));
        scope = allscopes.back();
        scopeDepthVector.push_back(0);
    }
    ;

closebrace_statement
    : RIGHT_BRACE { 
        if(!scopeDepthVector.empty())
            scopeDepthVector.pop_back();
        if(!allscopes.empty())
        {
            allscopes.pop_back();
            scope = allscopes.back();   
        }
    }
    ;

compound_statement
    : openbrace_statement statement_list closebrace_statement
    /* : LEFT_BRACE statement_list RIGHT_BRACE */
    ;

/* selective_statement
    : IF LEFT_PAREN expression RIGHT_PAREN compound_statement
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement ELSE selective_statement
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement ELSE compound_statement
    ; */

selective_statement
    : IF LEFT_PAREN expression RIGHT_PAREN compound_statement else_if 
    | IF LEFT_PAREN expression RIGHT_PAREN compound_statement else_if ELSE compound_statement
    ;

else_if
    :
    | else else_if 
    ;  

else:
    ELIF LEFT_PAREN expression RIGHT_PAREN compound_statement
    ;

iterative_statement
    : LOOP LEFT_PAREN expression RIGHT_PAREN compound_statement
    | LOOP LEFT_PAREN declaration SEMICOLON expression SEMICOLON assignment_expression RIGHT_PAREN compound_statement 
    | LOOP LEFT_PAREN assignment_expression SEMICOLON expression SEMICOLON assignment_expression RIGHT_PAREN compound_statement 
    ;

jump_statement
    : STOP SEMICOLON 
    | SKIP SEMICOLON 
    | RETURN SEMICOLON
    | RETURN expression SEMICOLON
    ;
%%

int yyerror(string s) {
    outputFile << endl << "Error: " << s << " at line " << yylineno << ".\n";
    tokenFile << endl << "Error: " << s << " at line " << yylineno << ".\n";
    cerr << "Error: " << s << " at line " << yylineno << ".\n";
    return 0;
}

int main(int argc, char *argv[]) {
    cout << "\033[1;34m" << "Starting Parser." << "\033[0m" << endl;
    if (argc < 4) {
        printf("Usage: %s <input file> <output file> <token file> <Verbose flag>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        fclose(inputFile);
        return 1;
    }

    outputFile.open(argv[2]);
    if(!outputFile.is_open()) {
        cerr << "Error: Cannot open output file " << argv[2] << endl;
        fclose(inputFile);
        outputFile.close();
        return 1;
    }

    tokenFile.open(argv[3]);
    if(!tokenFile.is_open()) {
        cerr << "Error: Cannot open token file " << argv[3] << endl;
        fclose(inputFile);
        tokenFile.close();
        outputFile.close();
        return 1;
    }

    yyin = inputFile;
    int parser_output = yyparse();
    if (parser_output == 0) {
        cout << "\033[1;32m" << "VERDICT: [SUCCESS]" << "\033[0m" << endl;
    } else {
        cerr << "\033[1;31m" << "VERDICT: [FAILED]" << "\033[0m" << endl;
    }

    fclose(inputFile);
    outputFile.close();
    tokenFile.close();
    cout << "\033[1;34m" << "Parser Finished." << "\033[0m" << endl;
    if(argc == 5 and !strcmp(argv[4], "-v")) {
        printSymbolTable(symbolTable);
    }
    return 0;
}