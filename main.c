#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include "historico.h"

#define MAX_DISCOS 10
#define NUM_TORRES 3

// -----------------------------
// Estrutura da Torre (Pilha)
typedef struct {
    int discos[MAX_DISCOS];
    int topo;
} Torre;

// Funções da pilha
void inicializarTorre(Torre *torre) { torre->topo = -1; }

int empilhar(Torre *torre, int disco) {
    if (torre->topo >= MAX_DISCOS - 1) return 0;
    torre->discos[++torre->topo] = disco;
    return 1;
}

int desempilhar(Torre *torre) {
    if (torre->topo < 0) return -1;
    return torre->discos[torre->topo--];
}

int topo(Torre *torre) {
    if (torre->topo < 0) return -1;
    return torre->discos[torre->topo];
}

// -----------------------------
// Funções auxiliares
void clear() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
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
    if (torreFinal->topo + 1 != numDiscos) return 0;
    for (int i = 0; i < numDiscos; i++) {
        if (torreFinal->discos[i] != numDiscos - i) return 0;
    }
    return 1;
}

// -----------------------------
// Funções de interação
void instrucoes() {
    clear();
    printf("           COMO JOGAR           \n");
    printf("================================\n\n");
    printf("Objetivo:\n");
    printf(" Mover todos os discos da haste inicial para a haste de destino.\n\n");
    printf("Como Jogar:\n");
    printf("  1. E possivel mexer somente um disco por vez.\n");
    printf("  2. So e possivel colocar um disco sobre outro quando a base for maior.\n\n");
    printf("Pressione 'Enter' para voltar ao menu!\n");
    getchar();
    getchar();
}

void salvarHistoricoEmArquivo(Historico* historico) {
    FILE* arquivo = fopen("historico.txt", "w");
    if (arquivo == NULL) return;

    Movimento* atual = historico->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "Disco %d: %c -> %c\n", atual->disco, atual->origem, atual->destino);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void exibirHistoricoDeArquivo() {
    FILE* arquivo = fopen("historico.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum historico encontrado.\n");
        return;
    }
    char linha[100];
    printf("\n==== Historico salvo ====\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        printf("%s", linha);
    }
    fclose(arquivo);
    printf("==========================\n");
    printf("Pressione Enter para voltar ao menu...");
    getchar(); getchar();
}

void jogar(int numDiscos) {
    Torre torres[NUM_TORRES];
    Historico* historico = criarHistorico();

    for (int i = 0; i < NUM_TORRES; i++) inicializarTorre(&torres[i]);
    for (int i = numDiscos; i >= 1; i--) empilhar(&torres[0], i);

    char origem, destino;
    while (1) {
        exibirTorres(torres, numDiscos);

        if (jogoConcluido(&torres[2], numDiscos)) {
            printf("\nParabens! Voce concluiu o jogo!\n");
            printf("\nMovimentos realizados:\n");
            exibirHistorico(historico);
            salvarHistoricoEmArquivo(historico);
            liberarHistorico(historico);
            break;
        }

        printf("\nDigite movimento (ex: AB), R para reiniciar, Q para sair: ");
        scanf(" %c", &origem);
        if (toupper(origem) == 'Q') {
            liberarHistorico(historico);
            break;
        }
        if (toupper(origem) == 'R') {
            liberarHistorico(historico);
            jogar(numDiscos);
            return;
        }
        scanf(" %c", &destino);

        int idxOrigem = indiceTorre(origem);
        int idxDestino = indiceTorre(destino);
        int disco = topo(&torres[idxOrigem]);

        if (idxOrigem == -1 || idxDestino == -1 ||
            disco == -1 ||
            (topo(&torres[idxDestino]) != -1 && topo(&torres[idxDestino]) < disco)) {
            printf("Movimento invalido! Pressione Enter...");
            getchar(); getchar();
            continue;
        }

        desempilhar(&torres[idxOrigem]);
        empilhar(&torres[idxDestino], disco);
        registrarMovimento(historico, disco, origem, destino);
    }
}

// -----------------------------
// Menu principal
int main() {
    setlocale(LC_ALL, "C");

    int opcao, numDiscos;

    do {
        clear();
        printf("            TORRE DE HANOI              \n");
        printf("======================================\n\n");
        printf("Escolha uma opcao:\n");
        printf("  1. Como Jogar\n");
        printf("  2. Jogar\n");
        printf("  3. Ver Historico Salvo\n");
        printf("  0. Sair do Jogo\n\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            instrucoes();
        }
        else if (opcao == 2) {
            do {
                printf("Escolha o numero de discos (1 a %d): ", MAX_DISCOS);
                scanf("%d", &numDiscos);
            } while (numDiscos < 1 || numDiscos > MAX_DISCOS);

            jogar(numDiscos);
            printf("Pressione Enter para voltar ao menu...");
            getchar(); getchar();
        }
        else if (opcao == 3) {
            exibirHistoricoDeArquivo();
        }
        else if (opcao == 0) {
            clear();
            printf("Obrigado por jogar!\n\n");
        }
        else {
            printf("\nOpcao invalida! Pressione Enter para tentar novamente!");
            getchar(); getchar();
        }

    } while (opcao != 0);

    return 0;
}
