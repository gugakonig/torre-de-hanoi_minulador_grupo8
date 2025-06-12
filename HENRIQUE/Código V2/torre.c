#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "torre.h"
#include "historico.h" // Inclui para usar as funções de histórico

void clear() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}

void inicializarTorre(Torre *torre) {
    (*torre).topo = -1;
}

int empilhar(Torre *torre, int disco) {
    if ((*torre).topo >= MAX_DISCOS - 1) return 0;
    (*torre).discos[++(*torre).topo] = disco;
    return 1;
}

int desempilhar(Torre *torre) {
    if ((*torre).topo < 0) return -1;
    return (*torre).discos[(*torre).topo--];
}

int topo(Torre *torre) {
    if ((*torre).topo < 0) return -1;
    return (*torre).discos[(*torre).topo];
}

void imprimirDisco(int tamanho, int larguraMax) {
    int larguraDisco = tamanho == 0 ? 1 : 2 * tamanho - 1;
    int espacos = (larguraMax - larguraDisco) / 2;

    for (int i = 0; i < espacos; i++) printf(" ");
    if (tamanho == 0)
        printf("|");
    else
        for (int i = 0; i < larguraDisco; i++) printf("*");
    for (int i = 0; i < espacos; i++) printf(" ");
}

void exibirTorres(Torre torres[], int numDiscos) {
    clear();
    printf("\nTorre de Hanoi - %d discos\n\n", numDiscos);

    for (int nivel = numDiscos - 1; nivel >= 0; nivel--) {
        for (int i = 0; i < NUM_TORRES; i++) {
            int disco = nivel <= torres[i].topo ? torres[i].discos[nivel] : 0;
            imprimirDisco(disco, 2 * numDiscos - 1);
            printf("   ");
        }
        printf("\n");
    }

    const char nomes[] = {'A', 'B', 'C'};
    for (int i = 0; i < NUM_TORRES; i++) {
        int largura = 2 * numDiscos - 1;
        for (int j = 0; j < largura / 2; j++) printf(" ");
        printf("%c", nomes[i]);
        for (int j = 0; j < largura / 2; j++) printf(" ");
        printf("   ");
    }
    printf("\n");
}

int indiceTorre(char letra) {
    letra = toupper(letra);
    if (letra == 'A') return 0;
    if (letra == 'B') return 1;
    if (letra == 'C') return 2;
    return -1;
}

int jogoConcluido(Torre *torreFinal, int numDiscos) {
    if ((*torreFinal).topo + 1 != numDiscos) return 0;
    for (int i = 0; i < numDiscos; i++) {
        if ((*torreFinal).discos[i] != numDiscos - i) return 0;
    }
    return 1;
}

void jogar(int numDiscos, const char *nome, const char *data, Historico **listaHistorico) {
    Torre torres[NUM_TORRES];
    for (int i = 0; i < NUM_TORRES; i++)
        inicializarTorre(&torres[i]);

    for (int i = numDiscos; i >= 1; i--)
        empilhar(&torres[0], i);

    char origem, destino;
    int movimentos = 0;

    while (1) {
        exibirTorres(torres, numDiscos);
        printf("\n\nMovimentos: %d\n", movimentos);

        if (jogoConcluido(&torres[2], numDiscos)) {
            printf("\nParabéns, %s! Jogo concluido com sucesso!\n", nome);
            
            // Chama as funções de histórico para salvar os dados
            salvarHistoricoArquivo(nome, data, numDiscos, movimentos);
            adicionarHistoricoLista(listaHistorico, nome, data, numDiscos, movimentos);
            
            printf("Histórico salvo. Pressione 'Enter' para continuar.");
            getchar(); getchar();
            break;
        }

        printf("\nDigite movimento (ex: AB), R para reiniciar, Q para sair: ");
        scanf(" %c", &origem);

        if (toupper(origem) == 'Q') break;
        if (toupper(origem) == 'R') {
            jogar(numDiscos, nome, data, listaHistorico); // Reinicia o jogo
            return;
        }

        scanf(" %c", &destino);

        int idxOrigem = indiceTorre(origem);
        int idxDestino = indiceTorre(destino);

        if (idxOrigem == -1 || idxDestino == -1) {
            printf("Torres inválidas. Pressione a tecla 'Enter' para tentar novamente!");
            getchar(); getchar();
            continue;
        }

        int disco = topo(&torres[idxOrigem]);
        if (disco == -1 || (topo(&torres[idxDestino]) != -1 && topo(&torres[idxDestino]) < disco)) {
            printf("Movimento inválido! Pressione a tecla 'Enter' para tentar novamente!");
            getchar(); getchar();
            continue;
        }

        desempilhar(&torres[idxOrigem]);
        empilhar(&torres[idxDestino], disco);
        movimentos++;
    }
}

void instrucoes() {
    clear();
    printf("           COMO JOGAR           \n");
    printf("================================\n\n");
    printf("Objetivo:\n");
    printf(" Mover todos os discos da haste inicial (A) para a haste de destino (C).\n\n");

    printf("Como Jogar:\n");
    printf("  1. É possível mexer somente um disco por vez.\n");
    printf("  2. Só é possível colocar um disco sobre outro quando a base for maior.\n\n");
    printf("Pressione 'Enter' para voltar ao menu principal!\n");
    getchar(); getchar();
}