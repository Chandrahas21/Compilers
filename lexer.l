%{
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <fstream>
    #include "parser.tab.hpp"
    using namespace std;

    void count();

    extern int yylineno;
    extern int yycolumnno;
    extern FILE *yyin;
    ofstream tokenFile, outputFile;
%}

comment_Start "#*"
comment_End "*#"

primitive_Data_Types ("void"|"string"|"bool")
non_Primitive_Data_Types ("num"|"point"|"equation"|"line"|"circle"|"parabola"|"hyperbola"|"ellipse"|"curve")

key_Words ("if"|"elif"|"el"|"loop"|"stop"|"skip"|"return"|"switch"|"goto"|"write"|"read"|"bool"|"struct"|"include"|"define"|"null"|"main"|"default"|"case"|"true"|"false")

punctuators (":"|";"|","|"\'"|"\""|"("|")"|"["|"]"|"{"|"}"|"<"|">"|"$"|"<=>")

special_Characters ("\\n"|"\\t")

operators ("++"|"--"|"!"|"^"|"*"|"/"|"%"|"+"|"-"|"=="|"!="|">="|"<="|"<"|">"|"&&"|"||"|"="|"~"|".")

in_Built ("to_string"|"distance"|"solve"|"sqrt"|"ispoint"|"intersection"|"tangent"|"slope_line"|"angle"|"type")

member_Variables ("_x"|"_y"|"_slope"|"_equation"|"_center"|"_radius"|"_a"|"_b"|"_c"|"_f"|"_g"|"_h"|"_delta"|"_eccentricity"|"_latus_rectum"|"_vertex"|"_focus")

%%

\#\#(.*) {
    count();
    tokenFile << yylineno << " : Single Line Comment : " << yytext << endl;
    outputFile << yytext;
    // return COMMENT_SINGLE_LINE;
}

{comment_Start}([^\*]|\*+[^\*\#])*\**{comment_End} {
    count();
    tokenFile << yylineno << " : Multi Line Comment : " << yytext << endl;
    outputFile << yytext;
    // return COMMENT_MULTI_LINE;
}

{primitive_Data_Types} {
    count();
    tokenFile << yylineno << " : Primitive Data Type : " << yytext << endl;
    outputFile << yytext;

    string primitive_data_type(yytext);
    if(primitive_data_type == "void") return VOID;
    else if(primitive_data_type == "string") return STRING;
    else if(primitive_data_type == "bool") return BOOL;
    else return -1;
}

{key_Words} {
    count();
    tokenFile << yylineno << " : Keyword : " << yytext << endl;
    outputFile << yytext;

    string key_word(yytext);
    if(key_word == "if") return IF;
    else if(key_word == "elif") return ELIF;
    else if(key_word == "el") return ELSE;
    else if(key_word == "loop") return LOOP;
    else if(key_word == "stop") return STOP;
    else if(key_word == "skip") return SKIP;
    else if(key_word == "return") return RETURN;
    else if(key_word == "write") return WRITE;
    else if(key_word == "read") return READ;
    else if(key_word == "bool") return BOOL;
    else if(key_word == "include") return INCLUDE;
    else if(key_word == "define") return DEFINE;
    else if(key_word == "null") return NULL_TOK;
    else if(key_word == "main") return MAIN;
    else if(key_word == "true") return TRUE;
    else if(key_word == "false") return FALSE;
    else return -1;
}

{non_Primitive_Data_Types} {
    count();
    tokenFile << yylineno << " : Non-Primitive Data Type : " << yytext << endl;
    outputFile << yytext;

    string non_primitive_data_type(yytext);
    if(non_primitive_data_type == "num") return NUM;
    else if(non_primitive_data_type == "point") return POINT;
    else if(non_primitive_data_type == "equation") return EQUATION;
    else if(non_primitive_data_type == "line") return LINE;
    else if(non_primitive_data_type == "circle") return CIRCLE;
    else if(non_primitive_data_type == "parabola") return PARABOLA;
    else if(non_primitive_data_type == "hyperbola") return HYPERBOLA;
    else if(non_primitive_data_type == "ellipse") return ELLIPSE;
    else if(non_primitive_data_type == "curve") return CURVE;
    else return -1;
}

{in_Built} {
    count();
    tokenFile << yylineno << " : In-Built Function : " << yytext << endl;
    outputFile << yytext;

    string in_built(yytext);
    if(in_built == "to_string") return TO_STRING;
    else if(in_built == "distance") return DISTANCE;
    else if(in_built == "solve") return SOLVE;
    else if(in_built == "sqrt") return SQRT;
    else if(in_built == "ispoint") return ISPOINT;
    else if(in_built == "intersection") return INTERSECTION;
    else if(in_built == "tangent") return TANGENT;
    else if(in_built == "slope_line") return SLOPE_LINE;
    else if(in_built == "angle") return ANGLE;
    else if(in_built == "type") return TYPE;
    else return -1;
}

{member_Variables} {
    count();
    tokenFile << yylineno << " : Member Variable : " << yytext << endl;
    outputFile << yytext;

    string member_variable(yytext);
    if(member_variable == "_x") return X;
    else if(member_variable == "_y") return Y;
    else if(member_variable == "_slope") return SLOPE;
    else if(member_variable == "_equation") return EQUATION_MV;
    else if(member_variable == "_center") return CENTER;
    else if(member_variable == "_radius") return RADIUS;
    else if(member_variable == "_a") return A;
    else if(member_variable == "_b") return B;
    else if(member_variable == "_c") return C;
    else if(member_variable == "_f") return F;
    else if(member_variable == "_g") return G;
    else if(member_variable == "_h") return H;
    else if(member_variable == "_delta") return DELTA;
    else if(member_variable == "_eccentricity") return ECCENTRICITY;
    else if(member_variable == "_latus_rectum") return LATUS_RECTUM;
    else if(member_variable == "_vertex") return VERTEX;
    else if(member_variable == "_focus") return FOCUS;
    else return -1;
}

{punctuators} {
    count();
    tokenFile << yylineno << " : Punctuator : " << yytext << endl;
    outputFile << yytext;

    string punctuator(yytext);
    if (punctuator == ":") return COLON;
    else if (punctuator == ";") return SEMICOLON;
    else if (punctuator == ",") return COMMA;
    else if (punctuator == "'") return SINGLE_QUOTE;
    else if (punctuator == "\"") return DOUBLE_QUOTE;
    else if (punctuator == "(") return LEFT_PAREN;
    else if (punctuator == ")") return RIGHT_PAREN;
    else if (punctuator == "[") return LEFT_BRACKET;
    else if (punctuator == "]") return RIGHT_BRACKET;
    else if (punctuator == "{") return LEFT_BRACE;
    else if (punctuator == "}") return RIGHT_BRACE;
    else if (punctuator == "<") return LESS_THAN;
    else if (punctuator == ">") return GREATER_THAN;
    else if (punctuator == "$") return DOLLAR;
    else if (punctuator == "<=>") return IMPLIES;
    else return -1;
}

{operators} {
    count();
    tokenFile << yylineno << " : Operator : " << yytext << endl;
    outputFile << yytext;

    string operator_var(yytext);
    if(operator_var == "++") return INCREMENT;
    else if(operator_var == "--") return DECREMENT;
    else if(operator_var == "!") return LOGICAL_NOT;
    else if(operator_var == "^") return EXPONENT;
    else if(operator_var == "*") return MULTIPLY;
    else if(operator_var == "/") return DIVIDE;
    else if(operator_var == "%") return MODULUS;
    else if(operator_var == "+") return PLUS;
    else if(operator_var == "-") return MINUS;
    else if(operator_var == "==") return EQUAL;
    else if(operator_var == "!=") return NOT_EQUAL;
    else if(operator_var == ">=") return GREATER_EQUAL;
    else if(operator_var == "<=") return LESS_EQUAL;
    else if(operator_var == "<") return LESS_THAN;
    else if(operator_var == ">") return GREATER_THAN;
    else if(operator_var == "&&") return LOGICAL_AND;
    else if(operator_var == "||") return LOGICAL_OR;
    else if(operator_var == "=") return ASSIGN;
    else if(operator_var == "~") return TILDE;
    else if(operator_var == ".") return MEMBER_ACCESS;
    else return -1;
}

{special_Characters} {
    count();
    tokenFile << yylineno << " : Special Character : " << yytext << endl;
    outputFile << yytext;

    string special_character(yytext);
    if(special_character == "\\n")
        return NEW_LINE_CHARACTER;
    else if(special_character == "\\t")
        return TAB_CHARACTER;
    else return -1;
}

[a-zA-Z][a-zA-Z0-9_]* {
    count();
    tokenFile << yylineno << " : Identifier : " << yytext << endl;
    outputFile << yytext;
    yylval.str_val = strdup(yytext);
    return IDENTIFIER;
}

([0-9]+)([eE][+-]?[0-9]+)? {
    count();
    tokenFile << yylineno << " : Integer : " << yytext << endl;
    outputFile << yytext;
    yylval.float_val = atoi(yytext);
    return CONSTANT;
}

([0-9]+[\.][0-9]+)([eE][+-]?[0-9]+)? {
    count();
    tokenFile << yylineno << " : Float : " << yytext << endl;
    outputFile << yytext;
    yylval.float_val = atof(yytext);
    return CONSTANT;
}

\"[^\"\n]*\" {
    count();
    tokenFile << yylineno << " : String Literal : " << yytext << endl;
    outputFile << yytext;
    yylval.str_val = strdup(yytext);
    return STRING_LITERAL;
}

[ \t\n] {
    count();
    /* Ignore whitespace */
    string temp(yytext);
    if(temp == "\n") {
        yylineno++;
    }
    outputFile << yytext;
}

. {
    // count();
    outputFile << "Lexical error: " << yytext << " at line no " << yylineno << "." << endl;
    tokenFile << "Lexical error: " << yytext << " at line no " << yylineno << "." << endl;
    cerr << "Lexical error: " << yytext << " at line " << yylineno << "." << endl;
    exit(0);
}

%%

int yywrap() {
    return 1;
}

int yycolumnno = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			yycolumnno = 0;
		else if (yytext[i] == '\t')
			yycolumnno += 8 - (yycolumnno % 8);
		else
			yycolumnno++;

	ECHO;
}