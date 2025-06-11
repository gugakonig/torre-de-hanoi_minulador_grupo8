#include "pilha.h"
#include <stdio.h>
#include <stdlib.h> // Para malloc e free

/**
 * @brief Cria e inicializa uma nova pilha (torre).
 * @param nome O caractere que identifica a pilha (ex: 'A', 'B', 'C').
 * @return Um ponteiro para a Pilha recém-criada, ou NULL em caso de erro de alocação.
 */
Pilha* criarPilha(char nome) {
    Pilha* novaPilha = (Pilha*) malloc(sizeof(Pilha));
    if (novaPilha == NULL) {
        perror("Erro ao alocar memoria para pilha");
        return NULL;
    }
    novaPilha->topo = NULL; // A pilha começa vazia
    novaPilha->nome = nome; // Atribui o nome à pilha
    return novaPilha;
}

/**
 * @brief Libera toda a memória alocada para uma pilha e seus nós.
 * @param pilha O ponteiro para a Pilha a ser liberada.
 */
void liberarPilha(Pilha* pilha) {
    if (pilha == NULL) {
        return;
    }
    No* atual = pilha->topo;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->abaixo;
        free(temp); // Libera cada nó (disco)
    }
    free(pilha); // Libera a estrutura da pilha em si
}

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha O ponteiro para a Pilha a ser verificada.
 * @return 1 se a pilha estiver vazia, 0 caso contrário.
 */
int pilhaVazia(Pilha* pilha) {
    return (pilha == NULL || pilha->topo == NULL);
}

/**
 * @brief Adiciona um disco ao topo da pilha.
 * * Realiza a validação da regra de um disco maior não poder ser colocado
 * * sobre um disco menor.
 * @param pilha O ponteiro para a Pilha onde o disco será empilhado.
 * @param tamanhoDisco O tamanho do disco a ser adicionado.
 */
void empilhar(Pilha* pilha, int tamanhoDisco) {
    if (pilha == NULL) {
        fprintf(stderr, "Erro: Pilha nao inicializada para empilhar.\n");
        return;
    }

    // Regra da Torre de Hanói: disco maior não pode ir em cima de disco menor
    if (pilha->topo != NULL && tamanhoDisco > pilha->topo->tamanhoDisco) {
        // Isso não deveria acontecer se a validação de movimento estiver correta no main.c,
        // mas é uma boa prática ter aqui também para robustez.
        fprintf(stderr, "Erro: Nao pode empilhar disco maior sobre disco menor.\n");
        return;
    }

    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para novo disco");
        return;
    }
    novoNo->tamanhoDisco = tamanhoDisco;
    novoNo->abaixo = pilha->topo; // O novo nó aponta para o antigo topo
    pilha->topo = novoNo;         // O novo nó se torna o topo
}

/**
 * @brief Remove e retorna o disco do topo da pilha.
 * @param pilha O ponteiro para a Pilha de onde o disco será desempilhado.
 * @return O tamanho do disco removido, ou -1 se a pilha estiver vazia.
 */
int desempilhar(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        // fprintf(stderr, "Erro: Pilha vazia, nao ha disco para desempilhar.\n"); // Pode ser um erro esperado no fluxo do jogo, então não é um erro fatal
        return -1; // Retorna -1 para indicar que não há disco
    }
    No* noRemovido = pilha->topo;       // Guarda o nó a ser removido
    int tamanho = noRemovido->tamanhoDisco; // Pega o tamanho do disco
    pilha->topo = noRemovido->abaixo;  // O topo agora é o próximo nó
    free(noRemovido);                  // Libera a memória do nó removido
    return tamanho;                    // Retorna o tamanho do disco
}

/**
 * @brief Retorna o tamanho do disco no topo da pilha sem removê-lo.
 * @param pilha O ponteiro para a Pilha.
 * @return O tamanho do disco no topo, ou -1 se a pilha estiver vazia.
 */
int topoDisco(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        return -1; // Retorna -1 se a pilha estiver vazia
    }
    return pilha->topo->tamanhoDisco; // Retorna o tamanho do disco no topo
}