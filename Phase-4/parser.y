/* %code requires{
    #include "traversal.hpp"
} */

%{
    #include <iostream>
    #include <string>
    #include <fstream>
    #include <cstring>
    using namespace std;

    extern int yylineno;
    extern int yycolumnno;
    extern int yylex();
    extern int yyerror(string s);

    extern FILE *yyin;
    extern ofstream tokenFile, outputFile;

    #include "symbolTable.hpp"
    #include <unordered_map>

    unordered_map<string, GlobalSymTabEntry> *globalSymbolTable = new unordered_map<string, GlobalSymTabEntry>();
    string isInFunction = "Global";
    string dataType = "";
    string functionName = "";
    string functionScope = "";
    vector<pair<string,string>> *functionArguments = new vector<pair<string,string>>();

    #include "scopeDepth.hpp"
    vector<int> scopeDepthVector = {};
    vector<string> allscopes = {};
    string scope = "";

    #include "traversal.hpp"
    class Start* start;
%}

%union {
    float float_val;
    bool bool_val;
    char *str_val;

    UnaryOperator TreeUnaryOperator;
    BinaryOperator TreeBinaryOperator;
    MemberVariable TreeMemberVariable;

    Node *TreeNode;

    Start *TreeStart;

    Header *TreeHeader;
    Program *TreeProgram;

    Statement *TreeStatement;

    InOut *TreeInOut;
    Scan *TreeScan;
    Print *TreePrint; 

    ConstantValue *TreeConstantValue;
    FunctionCall *TreeFunctionCall;
    BasicExpression *TreeBasicExpression;
    PostfixExpression *TreePostfixExpression;
    UnaryExpression *TreeUnaryExpression;
    AssignmentExpression *TreeAssignmentExpression;
    BinaryExpression *TreeBinaryExpression;
    Expression *TreeExpression;

    Declaration *TreeDeclarationType;
    DeclarationIndex *TreeDeclarationSetIndex;

    FunctionDeclaration *TreeFunctionDeclaration;
    FunctionArgumentList *TreeFunctionArgumentList;

    CompoundStatement *TreeCompoundStatement;
    ConditionalStatement *TreeSelectiveStatement;
    ElseIf *TreeElseIf;
    IterativeStatement *TreeIterativeStatement;
    JumpStatement *TreeJumpStatement;

    vector<Header *> *TreeHeaderList;
    vector<Program *> *TreeProgramList;
    vector<Statement *> *TreeStatementList;
    vector<Scan *> *TreeScanList;
    vector<Print *> *TreePrintList;
    vector<Expression *> *TreeDeclatarionExpressionList;
    vector<DeclarationIndex *> *TreeDeclarationIndex;
    vector<FunctionArgumentList *> *TreeFunctionArgList;
    vector<ElseIf *> *TreeElseIfList;
}

%token <str_val> VOID STRING BOOL TRUE FALSE
%token <str_val> IF ELIF ELSE 
%token <str_val> LOOP STOP SKIP RETURN
%token <str_val> WRITE READ INCLUDE DEFINE NULL_TOK MAIN DOLLAR TILDE MEMBER_ACCESS
%token <str_val> NUM POINT EQUATION LINE CIRCLE PARABOLA HYPERBOLA ELLIPSE CURVE
%token <str_val> TO_STRING DISTANCE SOLVE SQRT ISPOINT INTERSECTION TANGENT SLOPE_LINE ANGLE TYPE 
%token <str_val> X Y SLOPE EQUATION_MV RADIUS A B C F G H DELTA ECCENTRICITY LATUS_RECTUM VERTEX CENTER FOCUS    
%token <str_val> COLON SEMICOLON COMMA SINGLE_QUOTE DOUBLE_QUOTE IMPLIES
%token <str_val> LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE LESS_THAN GREATER_THAN 
%token <str_val> INCREMENT DECREMENT LOGICAL_NOT 
%token <str_val> EXPONENT MULTIPLY DIVIDE MODULUS PLUS MINUS 
%token <str_val> EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL LOGICAL_AND LOGICAL_OR ASSIGN 
%token <str_val> NEW_LINE_CHARACTER TAB_CHARACTER
%token <str_val> STRING_LITERAL 
%token <float_val> CONSTANT
%token <str_val> IDENTIFIER

%right ASSIGN   
%left LOGICAL_AND LOGICAL_OR
%left EQUAL NOT_EQUAL GREATER_EQUAL LESS_EQUAL GREATER_THAN LESS_THAN
%left PLUS MINUS
%left EXPONENT
%left MULTIPLY DIVIDE MODULUS

%type <TreeStart> start
%type <TreeHeader> header
%type <TreeProgram> program
%type <TreeStatement> statement
%type <TreeInOut> inOut
%type <TreePrint> print
%type <TreeMemberVariable> memberVariable
%type <TreeBasicExpression> basicExpression
%type <TreeFunctionCall> functionCall
%type <TreePostfixExpression> postfixExpression
%type <TreeUnaryOperator> unaryOperator
%type <TreeUnaryExpression> unaryExpression
%type <TreeAssignmentExpression> assignmentExpression
%type <TreeExpression> expression
%type <TreeDeclarationSetIndex> declarationIndex
%type <str_val> declarationType
%type <TreeDeclarationType> declaration
%type <str_val> defaultFunction
%type <str_val> functionIdentifier
%type <TreeFunctionDeclaration> functionDeclaration
%type <TreeStatement> statements
%type <TreeCompoundStatement> compoundStatement
%type <TreeSelectiveStatement> conditionalStatement
%type <TreeElseIf> elseIf
%type <TreeIterativeStatement> iterativeStatement
%type <TreeJumpStatement> jumpStatement

%type <TreeHeaderList> headerList
%type <TreeProgramList> programList
%type <TreeStatementList> statementList
%type <TreeScanList> scan
%type <TreePrintList> printList
%type <TreeDeclatarionExpressionList> declarationArgList
%type <TreeDeclarationIndex> declarationList
%type <TreeFunctionArgList> functionArgList
%type <TreeElseIfList> elseIfList

%%
start
    : headerList programList { $$ = new Start($1, $2, yylineno, yycolumnno); $$->globalSymbolTable = globalSymbolTable ; start = $$; }
    ;

headerList
    : header            { $$ = new vector<Header *>();  $$->push_back($1); }
    | header headerList { $2->push_back($1); $$ = $2; }
    ;

header
    : DOLLAR INCLUDE STRING_LITERAL SEMICOLON       { $$ = new Header(1, $3, yylineno, yycolumnno); }
    | DOLLAR DEFINE IDENTIFIER CONSTANT SEMICOLON   { $$ = new Header(0, $3, $4, yylineno, yycolumnno); }
    ;

programList
    : program               { $$ = new vector<Program *>(); $$->push_back($1); }
    | programList program   { $1->push_back($2); $$ = $1; }
    ;

program
    : setinDeclaration declaration SEMICOLON    { $$ = new Program(0, $2, yylineno, yycolumnno); }
    | setinFunction functionDeclaration         { $$ = new Program(1, $2, yylineno, yycolumnno); }
    ;

setinDeclaration
    : { isInFunction = "Global"; }
    ;

setinFunction
    : { isInFunction = "Function"; }
    ;

statement
    : assignmentExpression SEMICOLON   { $$ = new Statement(0, $1, yylineno, yycolumnno); }
    | declaration SEMICOLON            { $$ = new Statement(1, $1, yylineno, yycolumnno); }
    | inOut                            { $$ = new Statement(2, $1, yylineno, yycolumnno); }
    | statements                       { $$ = $1; }
    ;

statementList
    : statement                 { $$ = new vector<Statement *>(); $$->push_back($1); }
    | statement statementList   { $2->push_back($1); $$ = $2; }
    ;

inOut
    : WRITE TILDE printList SEMICOLON   { $$ = new InOut(1, $3, yylineno, yycolumnno); }
    | READ TILDE scan SEMICOLON         { $$ = new InOut(0, $3, yylineno, yycolumnno); }
    ;

scan
    : IDENTIFIER            { $$ = new vector<Scan *>(); $$->push_back(new Scan($1, scope, yylineno, yycolumnno)); }
    | IDENTIFIER TILDE scan { $3->push_back(new Scan($1, scope, yylineno, yycolumnno)); $$ = $3; }
    ;

print
    : NEW_LINE_CHARACTER    { $$ = new Print(0, strdup("\n"), yylineno, yycolumnno); }
    | TAB_CHARACTER         { $$ = new Print(0, strdup("\t"), yylineno, yycolumnno); }
    | postfixExpression     { $$ = new Print(1, $1, yylineno, yycolumnno); }
    ;

printList
    : print                 { $$ = new vector<Print *>(); $$->push_back($1); }
    | print TILDE printList { $3->push_back($1); $$ = $3; }
    ;

memberVariable
    : X             { $$ = MemberVariable::x; }
    | Y             { $$ = MemberVariable::y; }
    | SLOPE         { $$ = MemberVariable::slope; }
    | EQUATION_MV   { $$ = MemberVariable::equation; }
    | CENTER        { $$ = MemberVariable::center; }
    | FOCUS         { $$ = MemberVariable::focus; }
    | VERTEX        { $$ = MemberVariable::vertex; }
    | RADIUS        { $$ = MemberVariable::radius; }
    | A             { $$ = MemberVariable::a; }
    | B             { $$ = MemberVariable::b; }
    | C             { $$ = MemberVariable::c; }
    | F             { $$ = MemberVariable::f; }
    | G             { $$ = MemberVariable::g; }
    | H             { $$ = MemberVariable::h; }
    | DELTA         { $$ = MemberVariable::delta; }
    | ECCENTRICITY  { $$ = MemberVariable::eccentricity; }
    | LATUS_RECTUM  { $$ = MemberVariable::latus_rectum; }
    ;

basicExpression
    : IDENTIFIER                        { $$ = new BasicExpression(1, $1, scope, yylineno, yycolumnno); }
    | CONSTANT                          { $$ = new BasicExpression(0, new ConstantValue(0, $1, yylineno, yycolumnno), yylineno, yycolumnno); }
    | TRUE                              { $$ = new BasicExpression(0, new ConstantValue(1, true, yylineno, yycolumnno), yylineno, yycolumnno); }
    | FALSE                             { $$ = new BasicExpression(0, new ConstantValue(1, false, yylineno, yycolumnno), yylineno, yycolumnno); }
    | STRING_LITERAL                    { $$ = new BasicExpression(0, new ConstantValue(2, $1, yylineno, yycolumnno), yylineno, yycolumnno); }
    | LEFT_PAREN expression RIGHT_PAREN { $$ = new BasicExpression(2, $2, yylineno, yycolumnno); }
    ;

functionCall
    : IDENTIFIER LEFT_PAREN RIGHT_PAREN                             { $$ = new FunctionCall(0, $1, scope, yylineno, yycolumnno); }
	| IDENTIFIER LEFT_PAREN declarationArgList RIGHT_PAREN          { $$ = new FunctionCall(1, $1, $3, scope, yylineno, yycolumnno); }
    | defaultFunction LEFT_PAREN RIGHT_PAREN                        { $$ = new FunctionCall(2, $1, scope, yylineno, yycolumnno); }
    | defaultFunction LEFT_PAREN declarationArgList RIGHT_PAREN     { $$ = new FunctionCall(3, $1, $3, scope, yylineno, yycolumnno); }
    ;

postfixExpression
    : basicExpression                          { $$ = new PostfixExpression(0, $1, yylineno, yycolumnno); }
    | functionCall                             { $$ = new PostfixExpression(1, $1, yylineno, yycolumnno); }
    | IDENTIFIER MEMBER_ACCESS memberVariable  { $$ = new PostfixExpression(2, $1, $3, yylineno, yycolumnno); }
    | IDENTIFIER MEMBER_ACCESS memberVariable MEMBER_ACCESS memberVariable { $$ = new PostfixExpression(3, $1, $3, $5, yylineno, yycolumnno); }
    | postfixExpression INCREMENT              { $$->opList->push_back(UnaryOperator::inc_op); $$ = $1; }
    | postfixExpression DECREMENT              { $$->opList->push_back(UnaryOperator::dec_op); $$ = $1; }
    ;

unaryOperator
    : LOGICAL_NOT   { $$ = UnaryOperator::not_op; }
    | PLUS          { $$ = UnaryOperator::plus_op; }
    | MINUS         { $$ = UnaryOperator::minus_op; }
    | INCREMENT     { $$ = UnaryOperator::inc_op; }
    | DECREMENT     { $$ = UnaryOperator::dec_op; }
    ;
    
unaryExpression
    : postfixExpression             { $$ = new UnaryExpression($1, yylineno, yycolumnno); }
    | unaryOperator unaryExpression { $$->opList->push_back($1); $$ = $2; }
    ;

assignmentExpression
    : postfixExpression ASSIGN expression   { $$ = new AssignmentExpression(0, $1, $3, yylineno, yycolumnno); }
    | expression                            { $$ = new AssignmentExpression(1, $1, yylineno, yycolumnno); }
    ;

expression
    : unaryExpression                       { $$ = $1; $$->flagExpression = 1; }
    | expression MULTIPLY expression        { $$ = new BinaryExpression($1, $3, BinaryOperator::mul_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression DIVIDE expression          { $$ = new BinaryExpression($1, $3, BinaryOperator::div_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression MODULUS expression         { $$ = new BinaryExpression($1, $3, BinaryOperator::mod_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression EXPONENT expression        { $$ = new BinaryExpression($1, $3, BinaryOperator::exp_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression PLUS expression            { $$ = new BinaryExpression($1, $3, BinaryOperator::add_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression MINUS expression           { $$ = new BinaryExpression($1, $3, BinaryOperator::sub_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression EQUAL expression           { $$ = new BinaryExpression($1, $3, BinaryOperator::equal_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression NOT_EQUAL expression       { $$ = new BinaryExpression($1, $3, BinaryOperator::not_equal_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression GREATER_EQUAL expression   { $$ = new BinaryExpression($1, $3, BinaryOperator::greater_equal_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression LESS_EQUAL expression      { $$ = new BinaryExpression($1, $3, BinaryOperator::less_equal_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression GREATER_THAN expression    { $$ = new BinaryExpression($1, $3, BinaryOperator::less_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression LESS_THAN expression       { $$ = new BinaryExpression($1, $3, BinaryOperator::greater_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression LOGICAL_AND expression     { $$ = new BinaryExpression($1, $3, BinaryOperator::and_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    | expression LOGICAL_OR expression      { $$ = new BinaryExpression($1, $3, BinaryOperator::or_op , yylineno, yycolumnno); $$->flagExpression = 2; }
    ;

declarationType
    : VOID      { dataType = "Void"; $$ = strdup("Void"); }
    | STRING    { dataType = "String"; $$ = strdup("String"); }
    | BOOL      { dataType = "Bool"; $$ = strdup("Bool"); }
    | NUM       { dataType = "Num"; $$ = strdup("Num"); }
    | POINT     { dataType = "Point"; $$ = strdup("Point"); }
    | EQUATION  { dataType = "Equation"; $$ = strdup("Equation"); }
    | LINE      { dataType = "Line"; $$ = strdup("Line"); }
    | CIRCLE    { dataType = "Circle"; $$ = strdup("Circle"); }
    | PARABOLA  { dataType = "Parabola"; $$ = strdup("Parabola"); }
    | HYPERBOLA { dataType = "Hyperbola"; $$ = strdup("Hyperbola"); }
    | ELLIPSE   { dataType = "Ellipse"; $$ = strdup("Ellipse"); }
    | CURVE     { dataType = "Curve"; $$ = strdup("Curve"); }
    ;

declarationArgList
    : expression                            { $$ = new vector<Expression *>(); $$->push_back($1); }
    | expression COMMA declarationArgList   { $3->push_back($1); $$ = $3; }
    ;

declarationIndex
    : IDENTIFIER                                            { $$ = new DeclarationIndex(0, $1, yylineno, yycolumnno, scope); }
    | IDENTIFIER ASSIGN expression                          { $$ = new DeclarationIndex(1, $1, $3, yylineno, yycolumnno, scope); }
    | IDENTIFIER LEFT_PAREN declarationArgList RIGHT_PAREN  { $$ = new DeclarationIndex(2, $1, $3, yylineno, yycolumnno, scope); }
    ;

declarationList
    : declarationIndex                         { $$ = new vector<DeclarationIndex *>(); $$->push_back($1); }
    | declarationIndex COMMA declarationList   { $3->push_back($1); $$ = $3; }
    ;

declaration
    : declarationType declarationList { $$ = new Declaration($1, $2, strdup(isInFunction.c_str()), yylineno, yycolumnno); }
    ;

defaultFunction
    : TO_STRING     { $$ = strdup("ToString"); }
    | DISTANCE      { $$ = strdup("Distance"); }
    | SOLVE         { $$ = strdup("Solve"); }
    | SQRT          { $$ = strdup("Sqrt"); }
    | ISPOINT       { $$ = strdup("IsPoint"); }
    | INTERSECTION  { $$ = strdup("Intersection"); }
    | TANGENT       { $$ = strdup("Tangent"); }
    | SLOPE_LINE    { $$ = strdup("SlopeLine"); }
    | ANGLE         { $$ = strdup("Angle"); }
    | TYPE          { $$ = strdup("Type"); }
    | MAIN          { $$ = strdup("Main"); }
    ;

functionIdentifier
    : IDENTIFIER { $$ = strdup($1); }
    | MAIN { $$ = strdup("Main"); }
    ;

functionArgList
    : declarationType IDENTIFIER                        { auto temp = new FunctionArgumentList($1, $2, yylineno, yycolumnno); $$ = new vector<FunctionArgumentList *>(); $$->push_back(temp); }
    | declarationType IDENTIFIER COMMA functionArgList  { auto temp = new FunctionArgumentList($1, $2, yylineno, yycolumnno); $4->push_back(temp); $$ = $4; }
    ;

functionDeclaration
    : LESS_THAN declarationType GREATER_THAN functionIdentifier LEFT_PAREN RIGHT_PAREN IMPLIES compoundStatement                        { $$ = new FunctionDeclaration(2, $2, $4, $8, yylineno, yycolumnno); }
    | LESS_THAN declarationType GREATER_THAN functionIdentifier LEFT_PAREN functionArgList RIGHT_PAREN IMPLIES compoundStatement        { $$ = new FunctionDeclaration(3, $2, $4, $6, $9, yylineno, yycolumnno); }
    ;

statements
    : compoundStatement     { $$ = new Statement(3, $1, yylineno, yycolumnno); }
    | conditionalStatement  { $$ = new Statement(4, $1, yylineno, yycolumnno); }
    | iterativeStatement    { $$ = new Statement(5, $1, yylineno, yycolumnno); }
    | jumpStatement         { $$ = new Statement(6, $1, yylineno, yycolumnno); }
    ;

openbraceStatement
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

closebraceStatement
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

compoundStatement
    : openbraceStatement statementList closebraceStatement { $$ = new CompoundStatement($2, yylineno, yycolumnno); }
    /* : LEFT_BRACE statementList RIGHT_BRACE */
    ;

conditionalStatement
    : IF LEFT_PAREN expression RIGHT_PAREN compoundStatement elseIfList                         { $$ = new ConditionalStatement(0, $3, $5, $6, yylineno, yycolumnno); }
    | IF LEFT_PAREN expression RIGHT_PAREN compoundStatement elseIfList ELSE compoundStatement  { $$ = new ConditionalStatement(1, $3, $5, $6, $8, yylineno, yycolumnno); }
    ;

elseIfList
    :                   { $$ = new vector<ElseIf *>(); }
    | elseIf elseIfList { $2->push_back($1); $$ = $2; }
    ;  

elseIf:
    ELIF LEFT_PAREN expression RIGHT_PAREN compoundStatement { $$ = new ElseIf($3, $5, yylineno, yycolumnno); }
    ;

iterativeStatement
    : LOOP LEFT_PAREN expression RIGHT_PAREN compoundStatement                                                                  { $$ = new IterativeStatement(0, $3, $5, yylineno, yycolumnno); }
    | LOOP LEFT_PAREN declaration SEMICOLON expression SEMICOLON assignmentExpression RIGHT_PAREN compoundStatement             { $$ = new IterativeStatement(1, $3, $5, $7, $9, yylineno, yycolumnno); }
    | LOOP LEFT_PAREN assignmentExpression SEMICOLON expression SEMICOLON assignmentExpression RIGHT_PAREN compoundStatement    { $$ = new IterativeStatement(2, $3, $5, $7, $9, yylineno, yycolumnno); }
    ;

jumpStatement
    : STOP SEMICOLON                { $$ = new JumpStatement(0, yylineno, yycolumnno); }
    | SKIP SEMICOLON                { $$ = new JumpStatement(1, yylineno, yycolumnno); }
    | RETURN SEMICOLON              { $$ = new JumpStatement(2, yylineno, yycolumnno); }
    | RETURN expression SEMICOLON   { $$ = new JumpStatement(3, $2, yylineno, yycolumnno); }
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
    
    traversal(start);

    if(argc == 5 and !strcmp(argv[4], "-v")) {
        printGlobalSymTab(globalSymbolTable);
    }
    return 0;
}