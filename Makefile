CC	:= cc
CFLAGS := -g -lsqlite3

OBJ_DIR	:= obj
BIN_DIR	:= bin
OBJECTS	:= $(OBJ_DIR)/todo.o
INCLUDE	:= -lsqlite3

main: $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o $(BIN_DIR)/main

$(OBJECTS): todo.c todo.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c todo.c -o $(OBJ_DIR)/todo.o
