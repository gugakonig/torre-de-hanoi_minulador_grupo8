#ifndef TORRE_H
#define TORRE_H

// Inclui o cabeçalho de histórico para usar o tipo 'Historico'
#include "historico.h"

#define MAX_DISCOS 5
#define MIN_DISCOS 3
#define NUM_TORRES 3

// Estrutura para uma torre
typedef struct {
    int discos[MAX_DISCOS];
    int topo;
} Torre;

// Funções do Jogo
void clear();
void inicializarTorre(Torre *torre);
int empilhar(Torre *torre, int disco);
int desempilhar(Torre *torre);
int topo(Torre *torre);
void imprimirDisco(int tamanho, int larguraMax);
void exibirTorres(Torre torres[], int numDiscos);
int indiceTorre(char letra);
int jogoConcluido(Torre *torreFinal, int numDiscos);
void jogar(int numDiscos, const char *nome, const char *data, Historico **listaHistorico);
void instrucoes();

#endif