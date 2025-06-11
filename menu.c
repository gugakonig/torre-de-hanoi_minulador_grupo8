#include "menu.h"
#include "historico.h" // Necessário para exibirHistorico e outras funções do histórico
#include "pilha.h"     // Necessário para a função jogar
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessário para strlen, strcspn

// Variável global para armazenar o nome do jogador atual
char nomeJogadorAtual[50];

/**
 * @brief Limpa a tela do console, dependendo do sistema operacional.
 */
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Para Windows
#else
    system("clear"); // Para Linux/macOS
#endif
}

/**
 * @brief Limpa o buffer de entrada (stdin) para evitar leituras indesejadas.
 * * Usado após scanf ou antes de fgets para garantir que o buffer esteja vazio.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Exibe as instruções de como jogar Torre de Hanói.
 */
void exibirInstrucoes() {
    clearScreen();
    printf("\n--- Como Jogar Torre de Hanoi ---\n");
    printf("----------------------------------\n");
    printf("O objetivo do jogo e mover todos os discos de um pino inicial\n");
    printf("para um pino de destino, seguindo algumas regras:\n\n");
    printf("1. Apenas um disco pode ser movido por vez.\n");
    printf("2. Um disco maior nunca pode ser colocado em cima de um disco menor.\n");
    printf("3. Cada movimento consiste em pegar o disco superior de um pino\n");
    printf("   e coloca-lo no topo de outro pino.\n\n");
    printf("Voce digita o movimento como duas letras, por exemplo: 'AB'\n");
    printf("para mover o disco do pino A para o pino B.\n\n");
    printf("Pinos: A (origem), B (auxiliar), C (destino).\n");
    printf("----------------------------------\n");
    printf("Pressione Enter para voltar ao menu...");
    limparBufferEntrada(); // Garante que o buffer esteja limpo
    getchar(); // Espera o usuário pressionar Enter
}

/**
 * @brief Exibe o menu principal do jogo e gerencia as opções do usuário.
 */
void exibirMenuPrincipal() {
    int opcao;
    int numDiscos;

    // Inicializa o sistema de histórico global ao iniciar o programa
    inicializarHistoricoGlobal();

    // Loop principal do menu
    do {
        clearScreen();
        printf("\n--- Bem-vindo a Torre de Hanoi! ---\n");
        printf("-------------------------------------\n");
        printf("1. Iniciar Novo Jogo\n");
        printf("2. Como Jogar\n");
        printf("3. Ver Historico de Partidas\n");
        printf("0. Sair\n");
        printf("-------------------------------------\n");
        printf("Escolha uma opcao: ");

        // Garante que o buffer de entrada está limpo antes de ler a opção
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida. Digite um numero. Pressione Enter para continuar...");
            limparBufferEntrada(); // Limpa o buffer após a leitura falha
            getchar(); // Espera a confirmação do usuário
            continue; // Volta ao início do loop
        }
        limparBufferEntrada(); // Limpa o buffer após a leitura bem-sucedida do número

        switch (opcao) {
            case 1:
                clearScreen();
                printf("\n--- Iniciar Novo Jogo ---\n");
                printf("Digite seu nome: ");
                // fgets lê a linha inteira, incluindo o '\n'
                if (fgets(nomeJogadorAtual, sizeof(nomeJogadorAtual), stdin) == NULL) {
                    fprintf(stderr, "Erro ao ler o nome do jogador.\n");
                    continue;
                }
                // Remove o '\n' lido por fgets, se presente
                nomeJogadorAtual[strcspn(nomeJogadorAtual, "\n")] = '\0';

                // Loop para garantir que o número de discos seja válido
                do {
                    printf("Escolha o numero de discos (%d a %d): ", MIN_DISCOS, MAX_DISCOS);
                    if (scanf("%d", &numDiscos) != 1) {
                        printf("Entrada invalida. Digite um numero.\n");
                        limparBufferEntrada(); // Limpa o buffer após a leitura falha
                    } else if (numDiscos < MIN_DISCOS || numDiscos > MAX_DISCOS) {
                        printf("Numero de discos fora do intervalo permitido.\n");
                    } else {
                        break; // Sai do loop interno se a entrada for válida
                    }
                } while (1);
                limparBufferEntrada(); // Limpa o buffer após a leitura bem-sucedida do número de discos

                jogar(numDiscos); // Inicia o jogo
                break;
            case 2:
                exibirInstrucoes();
                break;
            case 3:
                clearScreen();
                exibirHistorico();
                printf("\nPressione Enter para voltar ao menu...");
                getchar(); // Espera o usuário pressionar Enter
                break;
            case 0:
                clearScreen();
                printf("\nObrigado por jogar! Ate mais!\n\n");
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter para tentar novamente...");
                getchar(); // Espera o usuário pressionar Enter
                break;
        }
    } while (opcao != 0);

    // Libera a memória alocada para o histórico global antes de sair do programa
    liberarHistoricoGlobal();
}