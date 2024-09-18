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

# Rule to build the target executable
$(TARGET): $(LEX_OUTPUT)
	$(CC) -o $(TARGET) $(LEX_OUTPUT)

# Rule to generate the lexer source file from the Flex file
$(LEX_OUTPUT): $(LEX_SOURCE)
	$(FLEX) $(LEX_SOURCE)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(LEX_OUTPUT) lex.yy.c

.PHONY: all clean
