CC = gcc
CFLAGS =-g -ansi -pedantic -Wall -Werror
LDFLAGS=-g

.PHONY: run clean

TARGET  = main
SOURCES = Calendar.c main.c

OBJECTS = $(SOURCES:.c=.o)


run: $(TARGET)
	./$(TARGET)
	@echo "Completed"

main.o: main.c Calendar.o
	$(CC) $(CFLAGS) -c main.c 

Calendar.o: Calendar.c Calendar.h
	$(CC) $(CFLAGS) -c Calendar.c

$(TARGET): $(OBJECTS)


clean:
	rm -f $(TARGET) *.o