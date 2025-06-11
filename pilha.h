#ifndef PILHA_H
#define PILHA_H

// Estrutura para representar um nó (disco) na pilha
typedef struct No {
    int tamanhoDisco;   // Tamanho do disco
    struct No *abaixo;  // Ponteiro para o nó abaixo (próximo disco na pilha)
} No;

// Estrutura para representar a pilha (torre)
typedef struct {
    No *topo;           // Ponteiro para o disco no topo da pilha
    char nome;          // Nome da torre (ex: 'A', 'B', 'C')
} Pilha;

// Protótipos das funções da Pilha
Pilha* criarPilha(char nome);
void liberarPilha(Pilha* pilha);
int pilhaVazia(Pilha* pilha);
void empilhar(Pilha* pilha, int tamanhoDisco);
int desempilhar(Pilha* pilha);
int topoDisco(Pilha* pilha);

#endif // PILHA_H