CC = gcc
CFLAGS = -Wall -Wextra -std=c11

BIN_PARENT = parent
BIN_CHILD = child

SRC_PARENT = parent.c
SRC_CHILD = child.c

.PHONY: all clean

all: $(BIN_PARENT) $(BIN_CHILD)

$(BIN_PARENT): $(SRC_PARENT)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_CHILD): $(SRC_CHILD)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_PARENT) $(BIN_CHILD)
