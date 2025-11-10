
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS=
SRC=$(wildcard src/*.c)
OBJDIR=obj
BINDIR=bin
BIN=$(BINDIR)/hms_project

# convert src/foo.c -> obj/foo.o
OBJ=$(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

all: $(BIN)

$(BIN): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean


