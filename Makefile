CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu99 -I./src
LDFLAGS =
TARGET  = ebnf_maker

SRCS = src/main.c \
       src/ast/ast.c \
       src/ast/ast_print.c \
       src/lexer/lexer.c \
       src/maker/maker.c \
       src/parser/parser.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

