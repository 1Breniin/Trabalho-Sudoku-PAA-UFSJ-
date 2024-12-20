# Makefile pra compilar os 2 programas gerando 2 executaveis
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L
DEPS = backtracking.h heuristica.h

# Alvos principais
all: backtracking heuristica

# Alvo para compilar backtracking
backtracking: backtracking.o
	$(CC) $(CFLAGS) -o backtracking backtracking.o

backtracking.o: backtracking.c backtracking.h
	$(CC) $(CFLAGS) -c backtracking.c

# Alvo para compilar heuristica
heuristica: heuristica.o
	$(CC) $(CFLAGS) -o heuristica heuristica.o

heuristica.o: heuristica.c heuristica.h
	$(CC) $(CFLAGS) -c heuristica.c

# Limpar arquivos gerados
clean:
	rm -f *.o backtracking heuristica