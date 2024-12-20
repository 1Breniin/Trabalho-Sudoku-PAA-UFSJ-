#include "backtracking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>  // Incluindo time.h


// Função para verificar se o número é válido na célula
int is_valid(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return 0;
    }
    int start_row = row - row % 3, start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[start_row + i][start_col + j] == num) return 0;
        }
    }
    return 1;
}

// Função de backtracking para resolver o Sudoku
int solve_sudoku(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (is_valid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solve_sudoku(grid)) return 1;
                        grid[row][col] = 0;
                    }
                }
                return 0; // Sem solução
            }
        }
    }
    return 1; // Solução encontrada
}

// Função para carregar múltiplos Sudokus do arquivo
int load_sudokus(const char *filename, int puzzles[MAX_PUZZLES][SIZE][SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    int puzzle_count = 0;
    int row = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Se linha for vazia, significa troca de puzzle
        if (strcmp(line, "\n") == 0) {
            if (row > 0) {
                puzzle_count++;
                row = 0;
            }
            continue;
        }

        // Parseia a linha para preencher a matriz
        for (int col = 0, idx = 0; col < SIZE; col++, idx += 2) {
            puzzles[puzzle_count][row][col] = (line[idx] == EMPTY) ? 0 : line[idx] - '0';
        }
        row++;
    }

    // Incrementa o contador para o último puzzle lido
    if (row > 0) puzzle_count++;

    fclose(file);
    return puzzle_count;
}

// Função para salvar os Sudokus resolvidos no arquivo
void save_sudokus(const char *filename, int puzzles[MAX_PUZZLES][SIZE][SIZE], int puzzle_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir arquivo de saída");
        exit(EXIT_FAILURE);
    }

    for (int p = 0; p < puzzle_count; p++) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (puzzles[p][i][j] == 0) {
                    fprintf(file, "%c ", EMPTY);
                } else {
                    fprintf(file, "%d ", puzzles[p][i][j]);
                }
            }
            fprintf(file, "\n");
        }
        if (p < puzzle_count - 1) fprintf(file, "\n");
    }

    fclose(file);
}

// Função para medir o tempo de execução
void measure_time(struct timeval *start, struct timeval *end, clock_t cpu_start, clock_t cpu_end) {
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Tempo de execução (relógio): %.6f segundos\n", elapsed);

    double cpu_time = (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;
    printf("Tempo de execução (CPU): %.6f segundos\n", cpu_time);
}

// Main
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int puzzles[MAX_PUZZLES][SIZE][SIZE];
    struct timeval start, end;
    clock_t cpu_start, cpu_end;

    // Carrega múltiplos Sudokus
    int puzzle_count = load_sudokus(input_file, puzzles);

    for (int p = 0; p < puzzle_count; p++) {
        printf("Resolvendo Sudoku #%d...\n", p + 1);

        // Medir tempo de execução de relógio
        gettimeofday(&start, NULL);
        cpu_start = clock();

        if (!solve_sudoku(puzzles[p])) {
            fprintf(stderr, "Sem solução para o Sudoku #%d.\n", p + 1);
        }

        cpu_end = clock();
        gettimeofday(&end, NULL);

        // Mostrar tempo
        measure_time(&start, &end, cpu_start, cpu_end);
    }

    // Salvar os resultados
    save_sudokus(output_file, puzzles, puzzle_count);

    printf("Todos os Sudokus resolvidos e salvos em '%s'.\n", output_file);

    return 0;
}