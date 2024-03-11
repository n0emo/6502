CC = gcc -std=c11
CFLAGS = -Wall -Wextra -pedantic -g
INCLUDES = .

mos6502: cpu.o mem.o main.o
	$(CC) $(CFLAGS) cpu.o mem.o main.o -o mos6502

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c

mem.o: mem.c
	$(CC) $(CFLAGS) -c mem.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f mos6502 *.o
