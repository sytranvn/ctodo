CC = cc
CFLAGS = -Wall -std=c2x -D __USE_XOPEN=700 -g
LDFLAGS = -lsqlite3
OBJFILES = todo.o main.o
TARGET = todo

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
