#ifndef PILHA_H
#define PILHA_H

// Este cabeçalho define as estruturas de dados e os protótipos de funções
// para a implementação de uma Pilha genérica (LIFO - Last In, First Out).
// No contexto do jogo Torre de Hanói, cada "Pilha" representa um dos pinos,
// e os "Nós" da pilha representam os discos empilhados.

// A constante NUMERO_DE_PINOS é usada em main.c e é definida lá.
// Se fosse usada diretamente aqui, precisaríamos de uma forward declaration ou de outro arquivo.

/**
 * @brief Define a estrutura para um Nó da pilha.
 * * Cada nó representa um disco individual na Torre de Hanói.
 * * É uma lista encadeada, onde 'abaixo' aponta para o disco que está diretamente abaixo.
 */
typedef struct No {
    int tamanhoDisco;   // O tamanho do disco (ex: 1 para o menor, 3 para um disco médio)
    struct No* abaixo;  // Ponteiro para o próximo nó (disco) na pilha (o que está "abaixo" dele)
} No;

/**
 * @brief Define a estrutura principal da Pilha.
 * * Representa um dos pinos da Torre de Hanói, que contém uma coleção de discos (Nós).
 */
typedef struct Pilha {
    char nome;          // O nome do pino (ex: 'A', 'B', 'C') para identificação visual
    No* topo;           // Ponteiro para o nó que está no "topo" da pilha (o disco visível e removível)
} Pilha;

// --- Protótipos das Funções da Pilha ---

/**
 * @brief Cria e inicializa uma nova estrutura de Pilha.
 * @param nomeDoPino O caractere identificador para esta pilha (ex: 'A').
 * @return Um ponteiro para a Pilha recém-criada, ou NULL em caso de falha.
 */
Pilha* criarPilha(char nomeDoPino);

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha O ponteiro para a Pilha a ser verificada.
 * @return 1 (verdadeiro) se a pilha estiver vazia, 0 (falso) caso contrário.
 */
int pilhaVazia(Pilha* pilha);

/**
 * @brief Retorna o tamanho do disco que está no topo da pilha.
 * @param pilha O ponteiro para a Pilha.
 * @return O valor inteiro do tamanho do disco no topo, ou -1 se a pilha estiver vazia.
 */
int topoDisco(Pilha* pilha);

/**
 * @brief Adiciona um novo disco no topo da pilha.
 * * Esta função não valida as regras do jogo (disco maior sobre menor).
 * Essa validação deve ser feita ANTES de chamar empilhar.
 * @param pilha O ponteiro para a Pilha onde o disco será inserido.
 * @param tamanhoDisco O tamanho do disco a ser empilhado.
 * @return 1 se o empilhamento foi bem-sucedido, 0 em caso de falha (ex: alocação de memória).
 */
int empilhar(Pilha* pilha, int tamanhoDisco);

/**
 * @brief Remove o disco do topo da pilha e o retorna.
 * * É importante verificar se a pilha não está vazia com 'pilhaVazia()' antes de desempilhar.
 * @param pilha O ponteiro para a Pilha de onde o disco será removido.
 * @return O tamanho do disco desempilhado, ou -1 se a pilha estiver vazia ou inválida.
 */
int desempilhar(Pilha* pilha);

/**
 * @brief Libera toda a memória alocada para uma Pilha e todos os seus Nós (discos).
 * * Deve ser chamada para evitar vazamentos de memória.
 * @param pilha O ponteiro para a Pilha a ser liberada.
 */
void liberarPilha(Pilha* pilha);

#endif // PILHA_H