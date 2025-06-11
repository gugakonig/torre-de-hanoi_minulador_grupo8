#ifndef MENU_H
#define MENU_H

// Este cabeçalho define os protótipos de funções e declarações de variáveis globais
// que são parte do módulo de interface do usuário (menu) do jogo Torre de Hanói.
// Ele permite que outros arquivos (.c) acessem e utilizem as funcionalidades do menu.

// Constantes globais relevantes para o menu
#define MAX_DISCOS_POSSIVEIS 10    // Limite superior de discos para uma partida.

// --- Variáveis Globais (Declaradas aqui, definidas em menu.c) ---

// Declaração da variável global para armazenar o nome do jogador da partida atual.
// É usada para registrar o nome no histórico de partidas.
extern char nomeJogadorAtual[50];

// --- Protótipos das Funções do Menu ---

/**
 * @brief Limpa a tela do console.
 * * Implementa a limpeza de tela de forma compatível com Windows e sistemas Unix-like.
 */
void clearScreen();

/**
 * @brief Limpa o buffer de entrada do teclado.
 * * Essencial para evitar problemas de leitura de caracteres residuais (como '\n') após certos inputs.
 */
void limparBufferEntrada();

/**
 * @brief Exibe as instruções detalhadas de como jogar a Torre de Hanói.
 * * Pausa a execução até que o usuário pressione Enter para retornar ao menu principal.
 */
void instrucoes();

/**
 * @brief Exibe o menu principal do jogo Torre de Hanói.
 * * Gerencia as opções do usuário (jogar, histórico, instruções, sair) e o fluxo do programa.
 * * Carrega o histórico de partidas ao iniciar e o salva ao finalizar o programa.
 */
void exibirMenuPrincipal();

#endif // MENU_H