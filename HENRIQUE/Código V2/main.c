#include <stdio.h>
#include <locale.h>
#include "torre.h"
#include "historico.h" // Inclui o novo cabeçalho de histórico

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    Historico *listaHistorico = NULL; // Inicializa a lista de histórico

    carregarHistorico(&listaHistorico); // Carrega o histórico do arquivo para a lista

    do {
        clear();
        printf("            TORRE DE HANÓI              \n");
        printf("======================================\n\n");
        printf("Escolha uma opção:\n");
        printf("  1. Como Jogar\n");
        printf("  2. Jogar\n");
        printf("  3. Ver Histórico\n");
        printf("  0. Sair do Jogo\n\n");
        printf("Digite sua opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            instrucoes();
        }
        else if (opcao == 2) {
            int numDiscos;
            char nome[50];
            char data[11];

            clear();
            printf("Digite seu nome: ");
            scanf(" %49[^\n]", nome);
            printf("Digite a data (DD/MM/AAAA): ");
            scanf(" %10s", data);


            do {
                printf("Escolha o número de discos (%d a %d): ", MIN_DISCOS, MAX_DISCOS);
                scanf("%d", &numDiscos);
            } while (numDiscos < MIN_DISCOS || numDiscos > MAX_DISCOS);
            
            jogar(numDiscos, nome, data, &listaHistorico);
        }
        else if (opcao == 3) {
            exibirHistorico(listaHistorico);
        }
        else if (opcao == 0) {
            clear();
            printf("Obrigado por jogar!\n\n");
        }
        else {
            printf("\nOpção inválida! Pressione a tecla 'Enter' para tentar novamente!");
            getchar();
            getchar();
        }

    } while (opcao != 0);

    liberarHistorico(listaHistorico); // Libera a memória alocada para a lista
    return 0;
}