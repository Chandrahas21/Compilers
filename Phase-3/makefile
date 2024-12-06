# Variables
BISON = bison
FLEX = flex
CC = gcc

# Files
LEXER = lexer.l
PARSER = parser.y
LEXER_C = lex.yy.c
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
TARGET = parser

# Rules
all: $(TARGET)

$(PARSER_C) $(PARSER_H): $(PARSER)
	$(BISON) -d $(PARSER)

$(LEXER_C): $(LEXER)
	$(FLEX) $(LEXER)

$(TARGET): $(LEXER_C) $(PARSER_C)
	$(CC) -o $(TARGET) $(LEXER_C) $(PARSER_C)

NUM ?= 1

TEST_DIR = ./Test
INPUT_FILE = $(TEST_DIR)/input$(NUM).txt
OUTPUT_FILE = $(TEST_DIR)/output$(NUM).txt
TOKEN_FILE = $(TEST_DIR)/token$(NUM).txt

run:
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE) $(TOKEN_FILE)	

clean:
	rm -f $(LEXER_C) $(PARSER_C) $(PARSER_H) $(TARGET) $(OUTPUT_FILE) $(TOKEN_FILE)

.PHONY: clean
