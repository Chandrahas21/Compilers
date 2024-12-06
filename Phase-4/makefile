BISON = bison
FLEX = flex
CC = g++
CFLAGS = -std=c++17

LEXER = lexer.l
PARSER = parser.y
LEXER_C = lex.yy.cc
PARSER_C = parser.tab.cpp
PARSER_H = parser.tab.hpp
TARGET = parser

SYMBOL_TABLE_C = symbolTable.cpp
SYMBOL_TABLE_H = symbolTable.hpp
SYMBOL_TABLE = symbolTable

SCOPE_C = scopeDepth.cpp
SCOPE_H = scopeDepth.hpp
SCOPE = scopeDepth

AST_C = ast.cpp
AST_H = ast.hpp
AST = ast

TRAVERSAL_C = traversal.cpp
TRAVERSAL_H = traversal.hpp
TRAVERSAL = traversal

TRANSPILE_C = transpiler.cpp
TRANSPILE_H = transpiler.hpp
TRANSPILE = transpiler

ERROR_C = error.cpp
ERROR_H = error.hpp
ERROR = error

all: $(TARGET)

$(LEXER_C): $(LEXER)
	$(FLEX) -o $(LEXER_C) $(LEXER)

$(PARSER_C) $(PARSER_H): $(PARSER)
	$(BISON) -d -o $(PARSER_C) $(PARSER)

$(SYMBOL_TABLE): $(SYMBOL_TABLE_C) $(SYMBOL_TABLE_H)
	$(CC) $(CFLAGS) -c -o $(SYMBOL_TABLE) $(SYMBOL_TABLE_C)

$(SCOPE): $(SCOPE_C) $(SCOPE_H)
	$(CC) $(CFLAGS) -c -o $(SCOPE) $(SCOPE_C)

$(AST): $(AST_C) $(AST_H)
	$(CC) $(CFLAGS) -c -o $(AST) $(AST_C)

$(TRAVERSAL): $(TRAVERSAL_C) $(TRAVERSAL_H)
	$(CC) $(CFLAGS) -c -o $(TRAVERSAL) $(TRAVERSAL_C)

$(TRANSPILE): $(TRANSPILE_C) $(TRANSPILE_H)
	$(CC) $(CFLAGS) -c -o $(TRANSPILE) $(TRANSPILE_C)

$(ERROR): $(ERROR_C) $(ERROR_H)
	$(CC) $(CFLAGS) -c -o $(ERROR) $(ERROR_C)

$(TARGET): $(LEXER_C) $(PARSER_C) $(SYMBOL_TABLE) $(SCOPE) $(AST) $(TRAVERSAL) $(TRANSPILE) $(ERROR)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEXER_C) $(PARSER_C) $(SYMBOL_TABLE) $(SCOPE) $(AST) $(TRAVERSAL) $(TRANSPILE) $(ERROR)

TEST_DIR = ./Test

NUM ?= 1
INPUT_FILE = $(TEST_DIR)/input$(NUM).txt
OUTPUT_FILE = $(TEST_DIR)/output.txt
TOKEN_FILE = $(TEST_DIR)/token.txt

VERBOSE ?= 

run:
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE) $(TOKEN_FILE) $(VERBOSE)

clean:
	rm -f $(LEXER_C) $(PARSER_C) $(PARSER_H) $(TARGET) $(SYMBOL_TABLE) $(SCOPE) $(AST) $(TRAVERSAL) $(TRANSPILE) $(OUTPUT_FILE) $(TOKEN_FILE) $(TEST_DIR)/*.cpp $(TEST_DIR)/log.txt $(ERROR)

# $(TARGET): $(LEXER_C) $(PARSER_C) $(SYMBOL_TABLE) $(SCOPE) $(AST) $(TRAVERSAL)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(LEXER_C) $(PARSER_C) $(SYMBOL_TABLE_C) $(SCOPE_C) $(AST_C)  $(TRAVERSAL_C) $(SYMBOL_TABLE) $(SCOPE) $(AST) $(TRAVERSAL)