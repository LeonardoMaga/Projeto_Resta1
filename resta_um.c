/* - Leonardo Magalhães 10417121
 * - Victor Luigi de Carvalho Scott 10418730
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// O tabuleiro é 9x9 mas o valor jogavel é 7x7
char tabuleiro[9][10] = {
    "#########",
    "###ooo###",
    "###ooo###",
    "#ooooooo#",
    "#ooo ooo#",
    "#ooooooo#",
    "###ooo###",
    "###ooo###",
    "#########"
};

char solucao[31][9][10];

// Variável que mostra ao utilizador que o programa não bloqueou
long long contador_iteracoes = 0;

// Vetores de direção: Cima, Baixo, Esquerda, Direita
int dl[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// Função para mostrar que o algoritmo está a processando
void mostrar_progresso() {
    contador_iteracoes++;
    if (contador_iteracoes % 5000000 == 0) {
        printf(".");
        fflush(stdout); // Força a impressão no ecrã
    }
}

// Função para copiar matrizes
void copiar_tabuleiro(char dest[9][10], char src[9][10]) {
    for (int i = 0; i < 9; i++) {
        strcpy(dest[i], src[i]);
    }
}

//Backtracking
bool backtrack(int pinos_restantes, int passo) {
    mostrar_progresso();

    //se sobrou apenas 1 pino
    if (pinos_restantes == 1) {
        // Verifica se o pino está  no centro
        if (tabuleiro[4][4] == 'o') {
            return true;
        }
        return false;
    }

    //possíveis jogadas e testar
    for (int l = 1; l < 8; l++) {
        for (int c = 1; c < 8; c++) {
            
            // Se encontrar um pino tenta mover
            if (tabuleiro[l][c] == 'o') {
                
                // Testa as 4 direções possíveis
                for (int dir = 0; dir < 4; dir++) {
                    int l_meio = l + dl[dir];
                    int c_meio = c + dc[dir];
                    int l_destino = l + 2 * dl[dir];
                    int c_destino = c + 2 * dc[dir];

                    if (tabuleiro[l_meio][c_meio] == 'o' && tabuleiro[l_destino][c_destino] == ' ') {
                        
                        // --- FAZER O MOVIMENTO  ---
                        tabuleiro[l][c] = ' ';
                        tabuleiro[l_meio][c_meio] = ' ';
                        tabuleiro[l_destino][c_destino] = 'o';

                        // Guardar o estado do tabuleiro para imprimir no fim
                        copiar_tabuleiro(solucao[passo], tabuleiro);

                        // --- CONQUISTAR ---
                        if (backtrack(pinos_restantes - 1, passo + 1)) {
                            return true; 
                        }

                        // --- RETROCEDER / BACKTRACKING ---
                        tabuleiro[l][c] = 'o';
                        tabuleiro[l_meio][c_meio] = 'o';
                        tabuleiro[l_destino][c_destino] = ' ';
                    }
                }
            }
        }
    }
    
    return false; 
}

// escreve o tabuleiro no ficheiro de saída
void imprimir_no_ficheiro(FILE *ficheiro, char tab[9][10]) {
    for (int i = 0; i < 9; i++) {
        fprintf(ficheiro, "%s\n", tab[i]);
    }
    fprintf(ficheiro, "\n");
}

int main() {
    printf("A iniciar a procura pela solucao do Resta Um...\n");
    printf("O processamento pode demorar alguns segundos");
    fflush(stdout);

    // Guarda a configuração inicial do tabuleiro
    char tabuleiro_inicial[9][10];
    copiar_tabuleiro(tabuleiro_inicial, tabuleiro);

    if (backtrack(32, 0)) {
        printf("\n\nSolucao encontrada, foram analisadas %lld possibilidades.\n", contador_iteracoes);
        
        // Criar o ficheiro de saída 
        FILE *ficheiro = fopen("restaum.out", "w");
        if (ficheiro == NULL) {
            printf("Erro ao criar o ficheiro de saida.\n");
            return 1;
        }

        fprintf(ficheiro, "O processamento pode demorar alguns segundos...\n");
        
        // Imprime tabuleiro inicial
        imprimir_no_ficheiro(ficheiro, tabuleiro_inicial);

        // Imprime movimentos até sobrar o último pino no centro
        for (int i = 0; i < 31; i++) {
            imprimir_no_ficheiro(ficheiro, solucao[i]);
        }

        fclose(ficheiro);
        printf("A sequencia de movimentos foi guardada no ficheiro 'restaum.out' com sucesso.\n");

    } else {
        printf("\n\nNenhuma solucao foi encontrada.\n");
    }

    return 0;
}
