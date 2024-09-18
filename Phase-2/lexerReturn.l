%{
    #include <stdio.h>
    #include <string.h>
    int yylex();
    int yywrap();
    extern FILE *yyin;
    FILE *tokfile;
    FILE *parsefile;

    #define COMMENT_SINGLE_LINE 1
    #define COMMENT_MULTI_LINE 2
    #define VOID 3
    #define STRING 4
    #define BOOL 5
    #define IF 6
    #define ELIF 7
    #define ELSE 8
    #define LOOP 9
    #define STOP 10
    #define SKIP 11
    #define RETURN 12
    #define SWITCH 13
    #define GOTO 14
    #define WRITE 15
    #define READ 16
    #define STRUCT 18
    #define INCLUDE 19
    #define DEFINE 20
    #define NULL_TOK 21
    #define MAIN 22
    #define DEFAULT 23
    #define CASE 24
    #define TRUE 25
    #define FALSE 26
    #define NUM 27
    #define POINT 28
    #define EQUATION 29
    #define LINE 30
    #define CIRCLE 31
    #define PARABOLA 32
    #define HYPERBOLA 33
    #define ELLIPSE 34
    #define CURVE 35
    #define TO_STRING 36
    #define DISTANCE 37
    #define SOLVE 38
    #define SQRT 39
    #define ISPOINT 40
    #define INTERSECTION 41
    #define TANGENT 42
    #define SLOPE_LINE 43
    #define ANGLE 44
    #define TYPE 45
    #define X 46
    #define Y 47
    #define SLOPE 48
    #define EQUATION_MV 49
    #define CENTER 50
    #define RADIUS 51
    #define A 52
    #define B 53
    #define C 54
    #define F 55
    #define G 56
    #define H 57
    #define DELTA 58
    #define ECCENTRICITY 59
    #define LATUS_RECTUM 60
    #define IDENTIFIER 61
    #define COLON 62
    #define SEMICOLON 63
    #define COMMA 64
    #define SINGLE_QUOTE 65
    #define DOUBLE_QUOTE 66
    #define LEFT_PAREN 67
    #define RIGHT_PAREN 68
    #define LEFT_BRACKET 69
    #define RIGHT_BRACKET 70
    #define LEFT_BRACE 71
    #define RIGHT_BRACE 72
    #define LESS_THAN 73
    #define GREATER_THAN 74
    #define DOLLAR 75
    #define INCREMENT 76
    #define DECREMENT 77
    #define LOGICAL_NOT 78
    #define EXPONENT 79
    #define MULTIPLY 80
    #define DIVIDE 81
    #define MODULUS 82
    #define PLUS 83
    #define MINUS 84
    #define EQUAL 85
    #define NOT_EQUAL 86
    #define GREATER_EQUAL 87
    #define LESS_EQUAL 88
    #define LOGICAL_AND 89
    #define LOGICAL_OR 90
    #define ASSIGN 91
    #define TILDE 92
    #define MEMBER_ACCESS 93
    #define NEW_LINE_CHARACTER 94
    #define TAB_CHARACTER 95
    #define INTEGER 96
    #define FLOATING_POINT_NUMBER 97
    #define STRING_LITERAL 98
%}

comment_Start "#*"
comment_End "*#"

primitive_Data_Types ("void"|"string"|"bool")
non_Primitive_Data_Types ("num"|"point"|"equation"|"line"|"circle"|"parabola"|"hyperbola"|"ellipse"|"curve")

key_Words ("if"|"elif"|"el"|"loop"|"stop"|"skip"|"return"|"switch"|"goto"|"write"|"read"|"bool"|"struct"|"include"|"define"|"null"|"main"|"default"|"case"|"true"|"false")

punctuators (":"|";"|","|"\'"|"\""|"("|")"|"["|"]"|"{"|"}"|"<"|">"|"$")

special_Characters ("\\n"|"\\t")

operators ("++"|"--"|"!"|"^"|"*"|"/"|"%"|"+"|"-"|"=="|"!="|">="|"<="|"<"|">"|"&&"|"||"|"="|"~"|".")

in_Built ("to_string"|"distance"|"solve"|"sqrt"|"ispoint"|"intersection"|"tangent"|"slope_line"|"angle"|"type")

member_Variables ("_x"|"_y"|"_slope"|"_equation"|"_center"|"_radius"|"_a"|"_b"|"_c"|"_f"|"_g"|"_h"|"_delta"|"_eccentricity"|"_latus_rectum")

%%

\#\#(.*) {
    fprintf(tokfile, "Single Line Comment: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return COMMENT_SINGLE_LINE;
}

{comment_Start}([^\*]|\*+[^\*\#])*\**{comment_End} {
    fprintf(tokfile, "Multi Line Comment: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return COMMENT_MULTI_LINE;
}

{primitive_Data_Types} {
    fprintf(tokfile, "Primitive Data Type: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    if(strcmp(yytext, "void") == 0) return VOID;
    else if(strcmp(yytext, "string") == 0) return STRING;
    else if(strcmp(yytext, "bool") == 0) return BOOL;
    else return -1;
}

{key_Words} {
    fprintf(tokfile, "Keyword: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    if(strcmp(yytext, "if") == 0) return IF;
    else if(strcmp(yytext, "elif") == 0) return ELIF;
    else if(strcmp(yytext, "el") == 0) return ELSE;
    else if(strcmp(yytext, "loop") == 0) return LOOP;
    else if(strcmp(yytext, "stop") == 0) return STOP;
    else if(strcmp(yytext, "skip") == 0) return SKIP;
    else if(strcmp(yytext, "return") == 0) return RETURN;
    else if(strcmp(yytext, "switch") == 0) return SWITCH;
    else if(strcmp(yytext, "goto") == 0) return GOTO;
    else if(strcmp(yytext, "write") == 0) return WRITE;
    else if(strcmp(yytext, "read") == 0) return READ;
    else if(strcmp(yytext, "bool") == 0) return BOOL;
    else if(strcmp(yytext, "struct") == 0) return STRUCT;
    else if(strcmp(yytext, "include") == 0) return INCLUDE;
    else if(strcmp(yytext, "define") == 0) return DEFINE;
    else if(strcmp(yytext, "null") == 0) return NULL_TOK;
    else if(strcmp(yytext, "main") == 0) return MAIN;
    else if(strcmp(yytext, "default") == 0) return DEFAULT;
    else if(strcmp(yytext, "case") == 0) return CASE;
    else if(strcmp(yytext, "true") == 0) return TRUE;
    else if(strcmp(yytext, "false") == 0) return FALSE;
    else return -1;
}

{non_Primitive_Data_Types} {
    fprintf(tokfile, "Non-Primitive Data Type: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if(strcmp(yytext, "num") == 0) return NUM;
    else if(strcmp(yytext, "point") == 0) return POINT;
    else if(strcmp(yytext, "equation") == 0) return EQUATION;
    else if(strcmp(yytext, "line") == 0) return LINE;
    else if(strcmp(yytext, "circle") == 0) return CIRCLE;
    else if(strcmp(yytext, "parabola") == 0) return PARABOLA;
    else if(strcmp(yytext, "hyperbola") == 0) return HYPERBOLA;
    else if(strcmp(yytext, "ellipse") == 0) return ELLIPSE;
    else if(strcmp(yytext, "curve") == 0) return CURVE;
    else return -1;
}

{in_Built} {
    fprintf(tokfile, "In-Built Function: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if(strcmp(yytext, "to_string") == 0) return TO_STRING;
    else if(strcmp(yytext, "distance") == 0) return DISTANCE;
    else if(strcmp(yytext, "solve") == 0) return SOLVE;
    else if(strcmp(yytext, "sqrt") == 0) return SQRT;
    else if(strcmp(yytext, "ispoint") == 0) return ISPOINT;
    else if(strcmp(yytext, "intersection") == 0) return INTERSECTION;
    else if(strcmp(yytext, "tangent") == 0) return TANGENT;
    else if(strcmp(yytext, "slope_line") == 0) return SLOPE_LINE;
    else if(strcmp(yytext, "angle") == 0) return ANGLE;
    else if(strcmp(yytext, "type") == 0) return TYPE;
    else return -1;
}

{member_Variables} {
    fprintf(tokfile, "Member Variable: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if(strcmp(yytext, "_x") == 0) return X;
    else if(strcmp(yytext, "_y") == 0) return Y;
    else if(strcmp(yytext, "_slope") == 0) return SLOPE;
    else if(strcmp(yytext, "_equation") == 0) return EQUATION_MV;
    else if(strcmp(yytext, "_center") == 0) return CENTER;
    else if(strcmp(yytext, "_radius") == 0) return RADIUS;
    else if(strcmp(yytext, "_a") == 0) return A;
    else if(strcmp(yytext, "_b") == 0) return B;
    else if(strcmp(yytext, "_c") == 0) return C;
    else if(strcmp(yytext, "_f") == 0) return F;
    else if(strcmp(yytext, "_g") == 0) return G;
    else if(strcmp(yytext, "_h") == 0) return H;
    else if(strcmp(yytext, "_delta") == 0) return DELTA;
    else if(strcmp(yytext, "_eccentricity") == 0) return ECCENTRICITY;
    else if(strcmp(yytext, "_latus_rectum") == 0) return LATUS_RECTUM;
    else return -1;
}

[a-zA-Z][a-zA-Z0-9_]* {
    fprintf(tokfile, "Identifier: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return IDENTIFIER;
}

{punctuators} {
    fprintf(tokfile, "Punctuation: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if (strcmp(yytext, ":") == 0) return COLON;
    else if (strcmp(yytext, ";") == 0) return SEMICOLON;
    else if (strcmp(yytext, ",") == 0) return COMMA;
    else if (strcmp(yytext, "'") == 0) return SINGLE_QUOTE;
    else if (strcmp(yytext, "\"") == 0) return DOUBLE_QUOTE;
    else if (strcmp(yytext, "(") == 0) return LEFT_PAREN;
    else if (strcmp(yytext, ")") == 0) return RIGHT_PAREN;
    else if (strcmp(yytext, "[") == 0) return LEFT_BRACKET;
    else if (strcmp(yytext, "]") == 0) return RIGHT_BRACKET;
    else if (strcmp(yytext, "{") == 0) return LEFT_BRACE;
    else if (strcmp(yytext, "}") == 0) return RIGHT_BRACE;
    else if (strcmp(yytext, "<") == 0) return LESS_THAN;
    else if (strcmp(yytext, ">") == 0) return GREATER_THAN;
    else if (strcmp(yytext, "$") == 0) return DOLLAR;
    else return -1;
}

{operators} {
    fprintf(tokfile, "Operator: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if(strcmp(yytext, "++") == 0) return INCREMENT;
    else if(strcmp(yytext, "--") == 0) return DECREMENT;
    else if(strcmp(yytext, "!") == 0) return LOGICAL_NOT;
    else if(strcmp(yytext, "^") == 0) return EXPONENT;
    else if(strcmp(yytext, "*") == 0) return MULTIPLY;
    else if(strcmp(yytext, "/") == 0) return DIVIDE;
    else if(strcmp(yytext, "%") == 0) return MODULUS;
    else if(strcmp(yytext, "+") == 0) return PLUS;
    else if(strcmp(yytext, "-") == 0) return MINUS;
    else if(strcmp(yytext, "==") == 0) return EQUAL;
    else if(strcmp(yytext, "!=") == 0) return NOT_EQUAL;
    else if(strcmp(yytext, ">=") == 0) return GREATER_EQUAL;
    else if(strcmp(yytext, "<=") == 0) return LESS_EQUAL;
    else if(strcmp(yytext, "<") == 0) return LESS_THAN;
    else if(strcmp(yytext, ">") == 0) return GREATER_THAN;
    else if(strcmp(yytext, "&&") == 0) return LOGICAL_AND;
    else if(strcmp(yytext, "||") == 0) return LOGICAL_OR;
    else if(strcmp(yytext, "=") == 0) return ASSIGN;
    else if(strcmp(yytext, "~") == 0) return TILDE;
    else if(strcmp(yytext, ".") == 0) return MEMBER_ACCESS;
    else return -1;
}

{special_Characters} {
    fprintf(tokfile, "Special Character: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);

    if (strcmp(yytext, "\\n") == 0 )
        return NEW_LINE_CHARACTER;
    else if(strcmp(yytext, "\\t") == 0)
        return TAB_CHARACTER;
    else return -1;
}

([0-9]+)([eE][+-]?[0-9]+)? {
    fprintf(tokfile, "Integer: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return INTEGER;
}

([0-9]+[\.][0-9]+)([eE][+-]?[0-9]+)? {
    fprintf(tokfile, "Floating Point Number: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return FLOATING_POINT_NUMBER;
}

\"[^\"^\n]*\" {
    fprintf(tokfile, "String Literal: %s\n", yytext);
    fprintf(parsefile, "%s\n", yytext);
    return STRING_LITERAL;
}

[ \t\n] {
    /* Ignore whitespace */
}

. {
    fprintf(tokfile, "Lexical error: %s\n", yytext);
    exit(1);
}

%%

int yywrap() {
    return 1;
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

    // yylex();

    int token = 0;
    while ((token = yylex()) != 0) {
        // printf("Token: %d\n", token);
    }

    fclose(inputFile);
    fclose(tokfile);
    fclose(parsefile);

    return 0;
}
