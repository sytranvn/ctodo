CC = cc
CFLAGS = -Wall
LDFLAGS = -lsqlite3
OBJFILES = todo.o main.o
TARGET = todo

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
