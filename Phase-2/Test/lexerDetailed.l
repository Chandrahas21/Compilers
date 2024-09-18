%{
    #include<stdio.h>
    #include<string.h>
    int yylex();
    int yywrap();
    extern FILE *yyin;
    FILE *outputFile;
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

\#\#(.*) { fprintf(outputFile, "Single Line Comment: %s\n", yytext); }
{comment_Start}([^\*]|\*+[^\*\#])*\**{comment_End} { fprintf(outputFile, "\nMulti Line Comment: %s\n\n", yytext); }

{primitive_Data_Types} { fprintf(outputFile, "Primitive Data Type: %s\n", yytext); }

{key_Words} { fprintf(outputFile, "Keyword: %s\n", yytext); }

{non_Primitive_Data_Types} { fprintf(outputFile, "Non-Primitive Data Type: %s\n", yytext); }

{in_Built} { fprintf(outputFile, "In-Built Function (Keyword): %s\n", yytext); }

{member_Variables} { fprintf(outputFile, "Member Variable (KeyWord): %s\n", yytext); }

[a-z|A-Z][a-z|A-Z|0-9|_]* { fprintf(outputFile, "Identifier: %s\n", yytext); }

{punctuators} { fprintf(outputFile, "Punctuator: %s\n", yytext); }

{operators} { fprintf(outputFile, "Operator: %s\n", yytext); }

{special_Characters} {fprintf(outputFile, "New line Character or Tab Character: %s\n",yytext); }

([0-9]+)([eE][+-]?[0-9]+)? { fprintf(outputFile, "Integer: %s\n", yytext); }
([0-9]+[\.][0-9]+)([eE][+-]?[0-9]+)? { fprintf(outputFile, "Floating Point Number: %s\n", yytext); }

\"[^\"^\n]*\" { fprintf(outputFile, "String: %s\n", yytext); }

[ \t\n] { /* do nothing */ fprintf(outputFile, "%s", yytext); }

. {fprintf(outputFile, "Lexical error: %s\n", yytext); exit(0); }

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        printf("Error: Cannot open output file %s\n", argv[2]);
        fclose(inputFile);
        return 1;
    }

    yyin = inputFile;

    yylex();

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}


// make 
// ./lexer input.txt output.txt