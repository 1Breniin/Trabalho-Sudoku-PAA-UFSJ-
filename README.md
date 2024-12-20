# Sudoku Solver

Este projeto implementa dois algoritmos para resolver instâncias de Sudoku: um método de força bruta baseado em backtracking e uma abordagem heurística utilizando MRV (Minimum Remaining Values). O objetivo é avaliar a eficiência de ambas as técnicas em diferentes cenários.

## Pré-requisitos

- Sistema operacional Linux.
- Compilador GCC.
- Biblioteca padrão de C.
- `make` instalado no sistema.

## Compilação

Para compilar o projeto, utilize o comando:

```bash
make

Este comando irá gerar os seguintes executáveis:

- backtracking
- heuristica  

## Execução

Os executáveis utilizam dois parâmetros obrigatórios:

1- Arquivo de entrada (.txt) contendo a configuração inicial do Sudoku.
2- Arquivo de saída (.txt) onde o resultado será gravado.

## Formato dos Arquivos

Entrada: Arquivo .txt com um tabuleiro de Sudoku, onde números representam valores fixos e v indica células vazias.
Exemplo:

v v 1 8 v v 3 v v
v 4 9 7 1 6 v 8 v
v 2 v v 9 v v v v
...

Saída: O arquivo gerado terá o mesmo formato da entrada, mas com as células vazias preenchidas.

## Exemplo de uso

./backtracking entrada.txt saida1.txt
./heuristica entrada.txt saida2.txt