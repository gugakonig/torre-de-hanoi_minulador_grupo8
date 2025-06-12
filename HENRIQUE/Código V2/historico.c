#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"
#include "torre.h" // Para a função clear()

// Adiciona uma sessão à lista encadeada em memória
void adicionarHistoricoLista(Historico **lista, const char *nome, const char *data, int modo, int movimentos) {
    Historico *novoNo = (Historico*)malloc(sizeof(Historico));
    if (!novoNo) {
        perror("Falha ao alocar memória para o histórico");
        return;
    }
    strncpy(novoNo->nomeJogador, nome, 49);
    novoNo->nomeJogador[49] = '\0';
    strncpy(novoNo->data, data, 10);
    novoNo->data[10] = '\0';
    novoNo->modoJogo = modo;
    novoNo->movimentos = movimentos;
    novoNo->proximo = *lista; // Insere no início da lista
    *lista = novoNo;
}

// Salva uma sessão de jogo no arquivo de texto "historico.txt"
void salvarHistoricoArquivo(const char *nome, const char *data, int modo, int movimentos) {
    FILE *arquivo = fopen("historico.txt", "a"); // Modo de anexar
    if (arquivo == NULL) {
        perror("Não foi possível abrir o arquivo de histórico");
        return;
    }
    fprintf(arquivo, "%s;%s;%d;%d\n", nome, data, modo, movimentos);
    fclose(arquivo);
}

// Carrega o histórico do arquivo para a lista encadeada ao iniciar o programa
void carregarHistorico(Historico **lista) {
    FILE *arquivo = fopen("historico.txt", "r");
    if (arquivo == NULL) {
        return; // Arquivo ainda não existe, o que é normal na primeira execução
    }
    char linha[256];
    char nome[50], data[11];
    int modo, movimentos;

    // Lê o arquivo linha por linha e preenche a lista
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%49[^;];%10[^;];%d;%d", nome, data, &modo, &movimentos) == 4) {
            adicionarHistoricoLista(lista, nome, data, modo, movimentos);
        }
    }
    fclose(arquivo);
}

// Exibe o histórico de partidas na tela
void exibirHistorico(Historico *lista) {
    clear();
    printf("         HISTÓRICO DE PARTIDAS         \n");
    printf("=======================================\n\n");
    if (lista == NULL) {
        printf("Nenhuma partida foi registrada ainda.\n");
    } else {
        printf("%-20s | %-12s | %-7s | %s\n", "Jogador", "Data", "Discos", "Movimentos");
        printf("------------------------------------------------------\n");
        Historico *atual = lista;
        while (atual != NULL) {
            printf("%-20s | %-12s | %-7d | %d\n",
                   atual->nomeJogador, atual->data, atual->modoJogo, atual->movimentos);
            atual = atual->proximo;
        }
    }
    printf("\n\nPressione 'Enter' para voltar ao menu principal!\n");
    getchar();
    getchar();
}

// Libera toda a memória alocada para a lista encadeada
void liberarHistorico(Historico *lista) {
    Historico *atual = lista;
    Historico *proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}