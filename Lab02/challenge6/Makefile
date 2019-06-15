CC=gcc
EXEC=challenge6

all: $(EXEC)

$(EXEC):  main.o
	$(CC) -o $@ $^ -lm

main.o: main.c
	$(CC) -c $< 

clean:
	rm -f *.o $(EXEC)
