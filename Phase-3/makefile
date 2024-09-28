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

clean:
	rm -f $(LEXER_C) $(PARSER_C) $(PARSER_H) $(TARGET)

.PHONY: clean
