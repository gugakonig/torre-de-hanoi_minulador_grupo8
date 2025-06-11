#include "historico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da variável global do histórico
HistoricoGlobal *historicoGlobal = NULL;

/**
 * @brief Inicializa a estrutura do histórico global de partidas.
 * * Deve ser chamada uma única vez no início do programa.
 */
void inicializarHistoricoGlobal() {
    historicoGlobal = (HistoricoGlobal*) malloc(sizeof(HistoricoGlobal));
    if (historicoGlobal == NULL) {
        perror("Erro ao alocar memoria para historicoGlobal");
        exit(EXIT_FAILURE); // Aborta o programa em caso de falha crítica de memória
    }
    historicoGlobal->inicio = NULL; // Inicializa a lista de partidas como vazia
    carregarHistoricoDeArquivo("historico.dat"); // Tenta carregar o histórico salvo
}

/**
 * @brief Cria e inicializa um novo objeto HistoricoMovimentos.
 * * Este objeto é usado para contabilizar os movimentos de uma única partida
 * * antes de ela ser adicionada ao histórico global.
 * @return Um ponteiro para a estrutura HistoricoMovimentos alocada, ou NULL em caso de erro.
 */
HistoricoMovimentos* criarHistoricoMovimentos() {
    HistoricoMovimentos* novoHistorico = (HistoricoMovimentos*) malloc(sizeof(HistoricoMovimentos));
    if (novoHistorico == NULL) {
        perror("Erro ao alocar memoria para HistoricoMovimentos");
        return NULL;
    }
    novoHistorico->numMovimentos = 0; // Inicia a contagem de movimentos em zero
    return novoHistorico;
}

/**
 * @brief Libera a memória alocada para uma estrutura HistoricoMovimentos.
 * @param historico O ponteiro para a estrutura HistoricoMovimentos a ser liberada.
 */
void liberarHistoricoMovimentos(HistoricoMovimentos* historico) {
    if (historico != NULL) {
        free(historico);
    }
}

/**
 * @brief Adiciona uma partida concluída ao histórico global.
 * * Cria um novo nó na lista encadeada e insere a partida.
 * * @param nomeJogador O nome do jogador da partida.
 * @param numDiscos O número de discos da partida.
 * @param historicoPartida O objeto HistoricoMovimentos com o total de movimentos da partida.
 */
void adicionarPartida(const char* nomeJogador, int numDiscos, HistoricoMovimentos* historicoPartida) {
    if (historicoGlobal == NULL) {
        fprintf(stderr, "Erro: Historico global nao inicializado.\n");
        return;
    }

    NoHistorico* novoNo = (NoHistorico*) malloc(sizeof(NoHistorico));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para NoHistorico");
        return;
    }

    strncpy(novoNo->partida.nomeJogador, nomeJogador, sizeof(novoNo->partida.nomeJogador) - 1);
    novoNo->partida.nomeJogador[sizeof(novoNo->partida.nomeJogador) - 1] = '\0'; // Garante null-termination
    novoNo->partida.numDiscos = numDiscos;
    novoNo->partida.numMovimentos = historicoPartida->numMovimentos; // Pega o total de movimentos da partida

    novoNo->proximo = historicoGlobal->inicio; // Adiciona no início da lista (mais recente primeiro)
    historicoGlobal->inicio = novoNo;

    salvarHistoricoEmArquivo("historico.dat"); // Salva o histórico após adicionar uma nova partida
}

/**
 * @brief Exibe todas as partidas registradas no histórico.
 */
void exibirHistorico() {
    if (historicoGlobal == NULL || historicoGlobal->inicio == NULL) {
        printf("\nNenhum historico de partidas disponivel.\n");
        return;
    }

    printf("\n--- Historico de Partidas ---\n");
    printf("-----------------------------\n");
    NoHistorico* atual = historicoGlobal->inicio;
    int contador = 1;
    while (atual != NULL) {
        printf("%d. Jogador: %s, Discos: %d, Movimentos: %d\n",
               contador++, atual->partida.nomeJogador, atual->partida.numDiscos, atual->partida.numMovimentos);
        atual = atual->proximo;
    }
    printf("-----------------------------\n");
}

/**
 * @brief Salva o histórico de partidas em um arquivo binário.
 * @param nomeArquivo O nome do arquivo onde o histórico será salvo.
 */
void salvarHistoricoEmArquivo(const char* nomeArquivo) {
    if (historicoGlobal == NULL) {
        return; // Nada para salvar se o histórico não foi inicializado
    }

    FILE* arquivo = fopen(nomeArquivo, "wb"); // Abre o arquivo em modo de escrita binária
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar historico");
        return;
    }

    NoHistorico* atual = historicoGlobal->inicio;
    while (atual != NULL) {
        // Escreve a estrutura Partida diretamente no arquivo
        fwrite(&(atual->partida), sizeof(Partida), 1, arquivo);
        atual = atual->proximo;
    }

    fclose(arquivo);
}

/**
 * @brief Carrega o histórico de partidas de um arquivo binário.
 * @param nomeArquivo O nome do arquivo de onde o histórico será carregado.
 */
void carregarHistoricoDeArquivo(const char* nomeArquivo) {
    if (historicoGlobal == NULL) {
        fprintf(stderr, "Erro: Historico global nao inicializado antes de carregar.\n");
        return;
    }

    // Libera qualquer histórico existente na memória antes de carregar um novo
    NoHistorico* atual = historicoGlobal->inicio;
    while (atual != NULL) {
        NoHistorico* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    historicoGlobal->inicio = NULL; // Garante que a lista está vazia

    FILE* arquivo = fopen(nomeArquivo, "rb"); // Abre o arquivo em modo de leitura binária
    if (arquivo == NULL) {
        // Se o arquivo não existe, não é um erro grave, apenas significa que não há histórico salvo ainda.
        // perror("Erro ao abrir arquivo para carregar historico"); // Descomente para depurar
        return;
    }

    Partida tempPartida;
    // Lê as estruturas Partida do arquivo uma por uma
    while (fread(&tempPartida, sizeof(Partida), 1, arquivo) == 1) {
        // Adiciona a partida lida ao histórico global (no início da lista)
        NoHistorico* novoNo = (NoHistorico*) malloc(sizeof(NoHistorico));
        if (novoNo == NULL) {
            perror("Erro ao alocar memoria ao carregar historico");
            // Se a alocação falhar durante o carregamento, pare e limpe o que foi carregado.
            liberarHistoricoGlobal(); 
            fclose(arquivo);
            return;
        }
        novoNo->partida = tempPartida;
        novoNo->proximo = historicoGlobal->inicio;
        historicoGlobal->inicio = novoNo;
    }

    fclose(arquivo);
}

/**
 * @brief Libera toda a memória alocada para o histórico global.
 * * Deve ser chamada no final do programa para evitar vazamentos de memória.
 */
void liberarHistoricoGlobal() {
    if (historicoGlobal == NULL) {
        return;
    }
    NoHistorico* atual = historicoGlobal->inicio;
    while (atual != NULL) {
        NoHistorico* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(historicoGlobal);
    historicoGlobal = NULL;
}