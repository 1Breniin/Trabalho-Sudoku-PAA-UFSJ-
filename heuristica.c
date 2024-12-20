#include "heuristica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

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

// Função para calcular o número de possibilidades para uma célula
int count_possibilities(int grid[SIZE][SIZE], int row, int col) {
    int count = 0;
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(grid, row, col, num)) count++;
    }
    return count;
}

// Função para encontrar a célula com menos possibilidades (heurística MRV)
Cell find_best_cell(int grid[SIZE][SIZE]) {
    Cell best_cell = {-1, -1, SIZE + 1};
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                int possibilities = count_possibilities(grid, row, col);
                if (possibilities < best_cell.possibilities) {
                    best_cell.row = row;
                    best_cell.col = col;
                    best_cell.possibilities = possibilities;
                }
            }
        }
    }
    return best_cell;
}

// Função de backtracking usando a heurística MRV
int heuristic_solve(int grid[SIZE][SIZE]) {
    Cell cell = find_best_cell(grid);
    if (cell.row == -1) return 1; // Sudoku resolvido

    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(grid, cell.row, cell.col, num)) {
            grid[cell.row][cell.col] = num;
            if (heuristic_solve(grid)) return 1;
            grid[cell.row][cell.col] = 0;
        }
    }
    return 0; // Sem solução
}

// Função de backtracking pura
int backtracking_solve(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (is_valid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (backtracking_solve(grid)) return 1;
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
int load_multiple_sudokus(const char *filename, int puzzles[][SIZE][SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }
    int puzzle_count = 0;
    while (!feof(file)) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                char ch;
                fscanf(file, " %c", &ch);
                puzzles[puzzle_count][i][j] = (ch == EMPTY) ? 0 : ch - '0';
            }
        }
        puzzle_count++;
        fgetc(file); // Ignora linhas em branco
        fgetc(file);
    }
    fclose(file);
    return puzzle_count;
}

// Função para salvar múltiplos Sudokus no arquivo
void save_multiple_sudokus(const char *filename, int puzzles[][SIZE][SIZE], int puzzle_count) {
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
        if (p < puzzle_count - 1) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

// Função para medir o tempo de CPU
void measure_cpu_time(clock_t start, clock_t end) {
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execução (CPU): %.6f segundos\n", elapsed);
}

// Função para medir o tempo de relógio
void measure_wall_time(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Tempo de execução (relógio): %.6f segundos\n", elapsed);
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    int puzzles[100][SIZE][SIZE];
    clock_t cpu_start, cpu_end;
    struct timeval wall_start, wall_end;

    // Carrega múltiplos Sudokus
    int puzzle_count = load_multiple_sudokus(input_file, puzzles);

    for (int p = 0; p < puzzle_count; p++) {
        printf("Resolvendo Sudoku #%d com heurística...\n", p + 1);

        // Tentar resolver com heurística
        gettimeofday(&wall_start, NULL);
        cpu_start = clock();
        int solved = heuristic_solve(puzzles[p]);
        cpu_end = clock();
        gettimeofday(&wall_end, NULL);

        if (!solved) {
            printf("Heurística falhou para Sudoku #%d, tentando backtracking...\n", p + 1);
            gettimeofday(&wall_start, NULL);
            cpu_start = clock();
            solved = backtracking_solve(puzzles[p]);
            cpu_end = clock();
            gettimeofday(&wall_end, NULL);
        }

        if (!solved) {
            fprintf(stderr, "Sem solução para o Sudoku #%d.\n", p + 1);
        } else {
            measure_cpu_time(cpu_start, cpu_end);
            measure_wall_time(&wall_start, &wall_end);
        }
    }

    save_multiple_sudokus(output_file, puzzles, puzzle_count);

    printf("Todos os Sudokus resolvidos e salvos em '%s'.\n", output_file);

    return 0;
}