%{
    #include <stdio.h>
    #include <string.h>
    #include "parser.tab.h"
    int yylex();
    int yywrap();
    extern FILE *yyin;
    FILE *tokfile;
    FILE *parsefile;
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

