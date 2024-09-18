%{
    #include<stdio.h>
    #include<string.h>
    int yylex();
    int yywrap();
    extern FILE *yyin;
    FILE *outputFile;
%}

primitive_Data_Types ("void"|"string"|"bool")
non_Primitive_Data_Types ("num"|"point"|"equation"|"line"|"circle"|"parabola"|"hyperbola"|"ellipse"|"curve")

key_Words ("if"|"elif"|"el"|"loop"|"stop"|"skip"|"return"|"switch"|"goto"|"write"|"read"|"bool"|"struct"|"include"|"define"|"null"|"main"|"default"|"case"|"true"|"false")

comment_Start "#*"
comment_End "*#"

punctuators (":"|";"|","|"\'"|"\""|"("|")"|"["|"]"|"{"|"}"|"<"|">"|"$")

special_Characters ("\\n"|"\\t")

operators ("++"|"--"|"!"|"^"|"*"|"/"|"%"|"+"|"-"|"=="|"!="|">="|"<="|"<"|">"|"&&"|"||"|"="|"~"|".")

in_Built ("to_string"|"distance"|"solve"|"sqrt"|"ispoint"|"intersection"|"tangent"|"slope_line"|"angle"|"type")

member_Variables ("x"|"y"|"slope"|"equation"|"center"|"radius"|"a"|"b"|"eccentricity"|"latus_rectum")

%%

\#\#(.*) { fprintf(outputFile, "Single Line Comment "); }
{comment_Start}([^\*]|\*+[^\*\#])*\**{comment_End} { fprintf(outputFile, "Multi Line Comment "); }

{primitive_Data_Types} { fprintf(outputFile, "Primitive Data Type "); }

{key_Words} { fprintf(outputFile, "Keyword "); }

{non_Primitive_Data_Types} { fprintf(outputFile, "Non-Primitive Data Type "); }

{in_Built} { fprintf(outputFile, "In-Built Function (Keyword) "); }

{member_Variables} { fprintf(outputFile, "Member Variable (Keyword) "); }

[a-z|A-Z][a-z|A-Z|0-9|_]* { fprintf(outputFile, "Identifier "); }

{punctuators} { fprintf(outputFile, "Punctuator "); }

{operators} { fprintf(outputFile, "Operator "); }

{special_Characters} {fprintf(outputFile, "New line Character or Tab Character "); }

([0-9]+)([eE][+-]?[0-9]+)? { fprintf(outputFile, "Integer "); }
([0-9]+[\.][0-9]+)([eE][+-]?[0-9]+)? { fprintf(outputFile, "Floating Point Number "); }

\"[^\"^\n]*\" { fprintf(outputFile, "String "); }

[ \t\n] { /* do nothing */ fprintf(outputFile, "%s" , yytext); }

. {fprintf(outputFile, "Lexical error "); exit(0); }

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