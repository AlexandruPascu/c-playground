CC=gcc
EXEC=challenge5

all: $(EXEC)

$(EXEC):  main.o  secret_functions.o
	$(CC) -o $@ $^

main.o: main.c
	$(CC) -c $<

secret_functions.o: secret_functions.c
	$(CC) -c $<

clean:
	rm -f *.o $(EXEC)
