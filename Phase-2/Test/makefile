# Makefile for compiling a Flex lexer

CC = gcc

# Flex tool
FLEX = flex

# Source files
LEX_SOURCE = lexerDetailed.l
LEX_OUTPUT = lex.yy.c

# Target executable
TARGET = lexerDetailed

# Default target
all: $(TARGET)

# Rule to generate the lexer source file from the Flex file
$(LEX_OUTPUT): $(LEX_SOURCE)
	$(FLEX) $(LEX_SOURCE)

# Rule to build the target executable
$(TARGET): $(LEX_OUTPUT)
	$(CC) -o $(TARGET) $(LEX_OUTPUT)

NUM ?= 1
INPUT_DIR = ./Input_Files
INPUT_FILE = $(INPUT_DIR)/input$(NUM).txt
OUTPUT_DIR = ./Output_Files
OUTPUT_FILE = $(OUTPUT_DIR)/output$(NUM).txt

run: 
	./$(TARGET) $(INPUT_FILE) $(OUTPUT_FILE)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(LEX_OUTPUT) lex.yy.c
