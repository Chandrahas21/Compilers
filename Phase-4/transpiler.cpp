#include "transpiler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void transpiler(Start *start) {
    ofstream file;
    file.open("./Test/output.cpp");
    file << cgStart(start);
    file.close();
}

string cgStart(Start *start) {
    string strStart = "";
    strStart += cgHeaderList(start->headerList);
    strStart += cgProgramList(start->programList);
    return strStart;
}

string cgHeaderList(vector<Header *> *headerList) {
    string strHeaderList = "";
    for (auto header : *headerList) {
        if (header->isHeader == 1) {
            strHeaderList += "#include " + string(header->header) + ";\n";
        } else {
            strHeaderList += "#define " + string(header->macroIdentifier) + " " + to_string(header->constantValue) + "\n";
        }
    }
    return strHeaderList;
}

string cgProgramList(vector<Program *> *programList) {
    string strProgramList = "";
    for (auto program : *programList) {
        if (program->isFunction == 0) {
            strProgramList += cgDeclaration(program->declaration);
        } else {
            strProgramList += cgFunctionDeclaration(program->functionDeclaration);
        }
    }
    return strProgramList;
}

string cgDeclaration(Declaration *declaration) {
    string strDeclaration = "";
    string dataType = string(declaration->declarationType);
    vector<DeclarationIndex *> *declarationList = declaration->declarationList;
    for (auto declarationItem : *declarationList) {
        string declarationIdentifier = string(declarationItem->declarationIdentifier);
        string declarationscope = string(declarationItem->scope);
        if (declarationItem->flagDeclarationIndex == 0) {
            strDeclaration += dataType + " " + declarationIdentifier + ";\n";
        } else if (declarationItem->flagDeclarationIndex == 1) {
            Expression *expression = declarationItem->expression;
            strDeclaration += dataType + " " + declarationIdentifier + " = " + cgExpression(expression) + ";\n";
        } else {
            vector<Expression *> *expressionList = declarationItem->expressionList;
            strDeclaration += dataType + " " + declarationIdentifier + " (";
            for (auto expression : *expressionList) {
                strDeclaration += cgExpression(expression);
                if (expression != expressionList->back()) {
                    strDeclaration += ", ";
                }
            }
            strDeclaration += ");\n";
        }
    }
    return strDeclaration;
}

string cgFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
    string strFunctionDeclaration = "";
    string returnType = string(functionDeclaration->returnType);
    string functionIdentifier = string(functionDeclaration->functonIdentifier);
    vector<FunctionArgumentList *> *argumentList = functionDeclaration->argumentList;
    CompoundStatement *compoundStatement = functionDeclaration->compoundStatement;

    strFunctionDeclaration += returnType + " " + functionIdentifier + "(";
    for (auto argumentItem : *argumentList) {
        strFunctionDeclaration += string(argumentItem->ArgType) + " " + string(argumentItem->ArgIdentifier);
        if (argumentItem != argumentList->back()) {
            strFunctionDeclaration += ", ";
        }
    }
    strFunctionDeclaration += ") {\n";
    strFunctionDeclaration += cgCompoundStatement(compoundStatement);
    strFunctionDeclaration += "}\n";
    return strFunctionDeclaration;
}

string cgCompoundStatement(CompoundStatement *compoundStatement) {
    string strCompoundStatement = "";
    vector<Statement *> *statementList = compoundStatement->statementList;
    for (auto statement : *statementList) {
        if (statement->flagStatement == 0) {
            strCompoundStatement += cgAssignmentExpression(statement->assignmentExpression);
        } else if (statement->flagStatement == 1) {
            strCompoundStatement += cgDeclaration(statement->declaration);
        } else if (statement->flagStatement == 2) {
            strCompoundStatement += cgInOut(statement->inOut);
        } else if (statement->flagStatement == 3) {
            strCompoundStatement += cgCompoundStatement(statement->compoundStatement);
        } else if (statement->flagStatement == 4) {
            cout << "Conditional --- Statement\n";
            strCompoundStatement += cgConditionalStatement(statement->conditionalStatement);
        } else if (statement->flagStatement == 5) {
            strCompoundStatement += cgIterativeStatement(statement->iterativeStatement);
        } else if (statement->flagStatement == 6) {
            strCompoundStatement += cgJumpStatement(statement->jumpStatement);
        }
    }
    return strCompoundStatement;
}

string cgInOut(InOut *inOut) {
    string strInOut = "";
    if (inOut->isWrite == 0) {
        vector<Scan *> *scanList = inOut->scanList;
        for (auto scan : *scanList) {
            strInOut += "cin >> " + string(scan->scanIdentifier) + ";\n";
        }
    } else {
        vector<Print *> *printList = inOut->printList;
        for (auto print : *printList) {
            if (print->flagPrint == 0) {
                strInOut += "cout << " + string(print->printIdentifier) + ";\n";
            } else {
                strInOut += "cout << " + cgExpression(print->expression) + ";\n";
            }
        }
    }
    return strInOut;
}

string cgAssignmentExpression(AssignmentExpression *assignmentExpression) {
    string strAssignmentExpression = "";
    if (assignmentExpression->flagExpression == 0) {
        strAssignmentExpression += cgPostFixExpression(assignmentExpression->postfixExpression) + " = " + cgExpression(assignmentExpression->expression) + ";\n";
    } else {
        strAssignmentExpression += cgExpression(assignmentExpression->expression) + ";\n";
    }
    return strAssignmentExpression;
}

string cgPostFixExpression(PostfixExpression *postfixExpression) {
    string strPostfixExpression = "";
    if (postfixExpression->flagPostfix == 0) {
        strPostfixExpression += cgBasicExpression(postfixExpression->basicExpression);
    } else {
        strPostfixExpression += cgFunctionCall(postfixExpression->functionCall);
    }
    return strPostfixExpression;
}

string cgBasicExpression(BasicExpression *basicExpression) {
    string strBasicExpression = "";
    if (basicExpression->flagBasic == 0) {
        strBasicExpression += cgConstantValue(basicExpression->constantValue);
    } else if (basicExpression->flagBasic == 1) {
        strBasicExpression += string(basicExpression->basicIdentifier);
    } else {
        strBasicExpression += cgExpression(basicExpression->expression);
    }
    return strBasicExpression;
}

string cgFunctionCall(FunctionCall *functionCall) {
    string strFunctionCall = "";
    strFunctionCall += string(functionCall->functionCallIdentifier) + "(";
    vector<Expression *> *argumentList = functionCall->argumentList;
    for (auto argument : *argumentList) {
        strFunctionCall += cgExpression(argument) + ", ";
    }
    strFunctionCall += ")";
    return strFunctionCall;
}

string cgConstantValue(ConstantValue *constantValue) {
    string strConstantValue = "";
    if (constantValue->flagConstant == 0) {
        strConstantValue += to_string(constantValue->fval);
    } else if (constantValue->flagConstant == 1) {
        strConstantValue += to_string(constantValue->bval);
    } else {
        strConstantValue += string(constantValue->sval);
    }
    return strConstantValue;
}

string cgExpression(Expression *expression) {
    string strExpression = "";
    if (expression->flagExpression == 0) {
        UnaryExpression *unaryExpression = static_cast<UnaryExpression *>(expression);
        strExpression += cgUnaryExpression(unaryExpression);
    } else {
        BinaryExpression *binaryExpression = static_cast<BinaryExpression *>(expression);
        strExpression += cgBinaryExpression(binaryExpression);
    }
    return strExpression;
}

string cgUnaryExpression(UnaryExpression *unaryExpression) {
    string strUnaryExpression = "";
    PostfixExpression *postfixExpression = unaryExpression->postfixExpression;
    vector<UnaryOperator> *opList = unaryExpression->opList;
    strUnaryExpression += cgPostFixExpression(postfixExpression);
    for (auto op : *opList) {
        if (op == UnaryOperator::inc_op) {
            strUnaryExpression += "++";
        } else if (op == UnaryOperator::dec_op) {
            strUnaryExpression += "--";
        } else if (op == UnaryOperator::not_op) {
            strUnaryExpression += "!";
        }
    }
    return strUnaryExpression;
}

string cgBinaryExpression(BinaryExpression *binaryExpression) {
    string strBinaryExpression = "";
    strBinaryExpression += cgExpression(binaryExpression->lhs);
    if (binaryExpression->op == BinaryOperator::add_op) {
        strBinaryExpression += " + ";
    } else if (binaryExpression->op == BinaryOperator::sub_op) {
        strBinaryExpression += " - ";
    } else if (binaryExpression->op == BinaryOperator::mul_op) {
        strBinaryExpression += " * ";
    } else if (binaryExpression->op == BinaryOperator::div_op) {
        strBinaryExpression += " / ";
    } else if (binaryExpression->op == BinaryOperator::mod_op) {
        strBinaryExpression += " % ";
    } else if (binaryExpression->op == BinaryOperator::and_op) {
        strBinaryExpression += " && ";
    } else if (binaryExpression->op == BinaryOperator::or_op) {
        strBinaryExpression += " || ";
    } else if (binaryExpression->op == BinaryOperator::equal_op) {
        strBinaryExpression += " == ";
    } else if (binaryExpression->op == BinaryOperator::not_equal_op) {
        strBinaryExpression += " != ";
    } else if (binaryExpression->op == BinaryOperator::less_op) {
        strBinaryExpression += " < ";
    } else if (binaryExpression->op == BinaryOperator::less_equal_op) {
        strBinaryExpression += " <= ";
    } else if (binaryExpression->op == BinaryOperator::greater_op) {
        strBinaryExpression += " > ";
    } else if (binaryExpression->op == BinaryOperator::greater_equal_op) {
        strBinaryExpression += " >= ";
    }
    strBinaryExpression += cgExpression(binaryExpression->rhs);
    return strBinaryExpression;
}

string cgConditionalStatement(ConditionalStatement *conditionalStatement) {
    string strConditionalStatement = "";

    strConditionalStatement += "if (" + cgExpression(conditionalStatement->expression) + ") {\n";
    strConditionalStatement += cgCompoundStatement(conditionalStatement->compoundStatement1);
    strConditionalStatement += "}\n";

    vector<ElseIf *> *elseIfList = conditionalStatement->elseIfList;
    for (auto elseIf : *elseIfList) {
        strConditionalStatement += "else if (" + cgExpression(elseIf->expression) + ") {\n";
        strConditionalStatement += cgCompoundStatement(elseIf->compoundStatement);
        strConditionalStatement += "}\n";
    }

    if (conditionalStatement->compoundStatement2 != NULL) {
        strConditionalStatement += "else {\n";
        strConditionalStatement += cgCompoundStatement(conditionalStatement->compoundStatement2);
        strConditionalStatement += "}\n";
    }
    return strConditionalStatement;
}

string cgIterativeStatement(IterativeStatement *iterativeStatement) {
    string strIterativeStatement = "";
    if (iterativeStatement->isWhile == 0) {
        strIterativeStatement += "while (" + cgExpression(iterativeStatement->expression2) + ") {\n";
        strIterativeStatement += cgCompoundStatement(iterativeStatement->compoundStatement);
        strIterativeStatement += "}\n";
    } else {
        if (iterativeStatement->declaration == NULL) {
            strIterativeStatement += "for (" + cgDeclaration(iterativeStatement->declaration) + "; " + cgExpression(iterativeStatement->expression2) + "; " + cgAssignmentExpression(iterativeStatement->assignmentexpression3) + ") {\n";
        } else {
            strIterativeStatement += "for (" + cgAssignmentExpression(iterativeStatement->assignmentexpression1) + "; " + cgExpression(iterativeStatement->expression2) + "; " + cgAssignmentExpression(iterativeStatement->assignmentexpression3) + ") {\n";
        }
        strIterativeStatement += cgCompoundStatement(iterativeStatement->compoundStatement);
        strIterativeStatement += "}\n";
    }
    return strIterativeStatement;
}

string cgJumpStatement(JumpStatement *jumpStatement) {
    string strJumpStatement = "";
    if (jumpStatement->flagJump == 0) {
        strJumpStatement += "break;\n";
    } else if (jumpStatement->flagJump == 1) {
        strJumpStatement += "continue;\n";
    } else {
        strJumpStatement += "return " + cgExpression(jumpStatement->expression) + ";\n";
    }
    return strJumpStatement;
}
