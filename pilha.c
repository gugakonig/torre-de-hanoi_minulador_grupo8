#include <stdio.h> // Para funções de entrada/saída (e.g., fprintf para erros)
#include <stdlib.h> // Para alocação/liberação de memória (malloc, free)

#include "pilha.h" // Inclui o cabeçalho com as definições das estruturas e protótipos

// Este arquivo implementa as operações básicas de uma estrutura de dados tipo Pilha (LIFO - Last In, First Out),
// que é fundamental para simular os pinos da Torre de Hanói e o empilhamento de discos.

/**
 * @brief Cria e inicializa uma nova pilha (pino).
 * * Aloca memória para a estrutura da Pilha e define seu nome, além de inicializar o topo como NULL.
 * @param nomeDoPino O caractere que identifica este pino (ex: 'A', 'B', 'C').
 * @return Um ponteiro para a Pilha recém-criada, ou NULL se a alocação de memória falhar.
 */
Pilha* criarPilha(char nomeDoPino) {
    // Aloca memória para a estrutura Pilha
    Pilha* novaPilha = (Pilha*)malloc(sizeof(Pilha));
    if (novaPilha == NULL) {
        // Se a alocação falhar, imprime um erro e retorna NULL.
        fprintf(stderr, "Erro: Falha na alocacao de memoria para a Pilha %c.\n", nomeDoPino);
        return NULL;
    }
    novaPilha->nome = nomeDoPino; // Define o nome do pino (A, B ou C)
    novaPilha->topo = NULL;       // Inicializa o topo da pilha como nulo, indicando que está vazia
    return novaPilha;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha O ponteiro para a pilha a ser verificada.
 * @return 1 se a pilha estiver vazia (topo é NULL), 0 caso contrário.
 */
int pilhaVazia(Pilha* pilha) {
    // É crucial verificar se a pilha em si não é nula antes de tentar acessar seu topo.
    if (pilha == NULL) {
        fprintf(stderr, "Aviso: Tentativa de verificar Pilha NULL.\n");
        return 1; // Consideramos uma pilha NULL como vazia para evitar crash.
    }
    return (pilha->topo == NULL); // Retorna 1 (verdadeiro) se o topo é nulo, 0 (falso) caso contrário.
}

/**
 * @brief Retorna o tamanho do disco no topo da pilha.
 * @param pilha O ponteiro para a pilha.
 * @return O tamanho do disco no topo da pilha (um inteiro), ou -1 se a pilha estiver vazia
 * ou se o ponteiro da pilha for inválido.
 */
int topoDisco(Pilha* pilha) {
    if (pilhaVazia(pilha)) { // Reutiliza a função pilhaVazia para clareza
        return -1; // Retorna -1 para indicar que não há disco no topo (pilha vazia).
    }
    return pilha->topo->tamanhoDisco; // Retorna o tamanho do disco que está no topo.
}

/**
 * @brief Adiciona (empilha) um disco no topo da pilha.
 * * A regra da Torre de Hanói de não colocar um disco maior sobre um menor
 * deve ser validada ANTES de chamar esta função. Esta função apenas realiza a operação de empilhamento.
 * @param pilha O ponteiro para a pilha onde o disco será empilhado.
 * @param tamanhoDisco O tamanho do disco a ser adicionado.
 * @return 1 se o empilhamento foi bem-sucedido, 0 se a alocação de memória falhar.
 */
int empilhar(Pilha* pilha, int tamanhoDisco) {
    // Primeiro, verifica se o ponteiro da pilha é válido.
    if (pilha == NULL) {
        fprintf(stderr, "Erro: Tentativa de empilhar em uma Pilha NULL.\n");
        return 0;
    }

    // Aloca memória para o novo nó (disco)
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        // Se a alocação falhar, imprime um erro e retorna 0.
        fprintf(stderr, "Erro: Falha na alocacao de memoria para novo disco.\n");
        return 0;
    }
    novoNo->tamanhoDisco = tamanhoDisco; // Define o tamanho do disco para este nó

    // O novo nó (disco) sempre aponta para o antigo topo da pilha, tornando-se o novo topo.
    novoNo->abaixo = pilha->topo; 
    pilha->topo = novoNo; // Atualiza o ponteiro 'topo' da pilha para o novo nó.

    return 1; // Retorna 1 para indicar sucesso.
}

/**
 * @brief Remove (desempilha) o disco do topo da pilha.
 * * É importante verificar se a pilha não está vazia ANTES de chamar esta função.
 * @param pilha O ponteiro para a pilha de onde o disco será desempilhado.
 * @return O tamanho do disco removido, ou -1 se a pilha estiver vazia ou for inválida.
 */
int desempilhar(Pilha* pilha) {
    // Primeiro, verifica se o ponteiro da pilha é válido e se ela não está vazia.
    if (pilhaVazia(pilha)) {
        fprintf(stderr, "Aviso: Tentativa de desempilhar de uma Pilha vazia ou NULL.\n");
        return -1; // Retorna -1 para indicar falha (nenhum disco para remover).
    }

    No* noParaRemover = pilha->topo; // Guarda o ponteiro para o nó que será removido
    int tamanhoDiscoRemovido = noParaRemover->tamanhoDisco; // Pega o tamanho do disco a ser retornado

    // O topo da pilha agora aponta para o nó que estava abaixo do anterior.
    pilha->topo = noParaRemover->abaixo; 
    free(noParaRemover); // Libera a memória do nó que foi removido

    return tamanhoDiscoRemovido; // Retorna o tamanho do disco que foi desempilhado.
}

/**
 * @brief Libera toda a memória alocada para uma pilha, incluindo todos os seus discos.
 * * Percorre a pilha de cima para baixo, liberando cada nó (disco) individualmente.
 * * Essencial para evitar vazamentos de memória ao final do programa ou de uma partida.
 * @param pilha O ponteiro para a pilha a ser liberada.
 */
void liberarPilha(Pilha* pilha) {
    if (pilha == NULL) return; // Não há nada para liberar se a pilha já é NULL.

    No* noAtual = pilha->topo;
    No* proximoNo; // Usado para guardar o ponteiro do próximo nó antes de liberar o atual

    // Percorre todos os nós da pilha, liberando um por um
    while (noAtual != NULL) {
        proximoNo = noAtual->abaixo; // Guarda o ponteiro para o próximo nó
        free(noAtual);             // Libera a memória do nó atual (disco)
        noAtual = proximoNo;       // Move para o próximo nó
    }
    free(pilha); // Finalmente, libera a memória da própria estrutura Pilha
}