#ifndef MENU_H
#define MENU_H

// Constantes para limites do número de discos
#define MIN_DISCOS 3
#define MAX_DISCOS 10

// Variável global para armazenar o nome do jogador atual
// (declarada aqui e definida em menu.c)
extern char nomeJogadorAtual[50]; 

// Protótipos das funções do menu
void clearScreen();
void limparBufferEntrada();
void exibirMenuPrincipal();
void exibirInstrucoes();

#endif // MENU_H