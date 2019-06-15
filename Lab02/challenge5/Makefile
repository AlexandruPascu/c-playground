CC=gcc
EXEC=challenge5

all: $(EXEC)

$(EXEC):  main.o
	$(CC) -o $@ $^ -lm

main.o: main.c
	$(CC) -c $< 

clean:
	rm -f *.o $(EXEC)
