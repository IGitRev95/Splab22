compC = gcc -m32 -g -Wall -o

.PHONY = all clean

all: encoder

encoder: encoder.o
	$(compC) encoder encoder.o

encoder.o: encoder.c
	gcc -m32 -g -Wall -c -o encoder.o encoder.c

clean:
	rm encoder.o encoder