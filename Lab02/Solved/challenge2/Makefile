CC=gcc
CFLAGS=-std=c99
EXEC=challenge2

all: $(EXEC)

$(EXEC):  main.o
	$(CC) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXEC)
