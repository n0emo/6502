CC = gcc -std=c11
CFLAGS = -Wall -Wextra -pedantic -g
INCLUDES = .

mos6502: cpu.o mem.o main.o int.o
	$(CC) $(CFLAGS) cpu.o mem.o int.o main.o -o mos6502

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c

mem.o: mem.c
	$(CC) $(CFLAGS) -c mem.c

int.o: int.c
	$(CC) $(CFLAGS) -c int.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f mos6502 *.o
