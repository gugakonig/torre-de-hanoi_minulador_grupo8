#include <stdio.h>    // Para funções de entrada/saída como printf, fgets, getchar
#include <stdlib.h>   // Para funções gerais como system (para limpar a tela) e atoi
#include <locale.h>   // Para setlocale, permitindo caracteres especiais em português
#include <string.h>   // Para funções de string como strcspn (para remover o '\n' do fgets)

// Inclusão dos cabeçalhos das outras partes do projeto
#include "menu.h"      // Inclui as definições e protótipos específicos do menu
#include "historico.h" // Inclui as funções de histórico para salvar, carregar e exibir partidas

// Declaração da função 'jogar' que está definida em main.c.
// Precisamos declará-la aqui para que 'exibirMenuPrincipal' possa chamá-la.
void jogar(int numDiscos);

// DEFINIÇÃO DA VARIÁVEL GLOBAL que armazena o nome do jogador da partida atual.
// Esta variável é 'extern' em main.c para ser acessada lá.
char nomeJogadorAtual[50];

/**
 * @brief Limpa a tela do console de forma compatível com diferentes sistemas operacionais.
 * * Usa 'cls' para Windows e sequências de escape ANSI para sistemas tipo Unix (Linux, macOS).
 */
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Comando para limpar a tela no Windows
#else
    // Sequência ANSI para limpar a tela e mover o cursor para a posição (1,1)
    printf("\033[2J\033[H");
    fflush(stdout); // Garante que a saída seja imediatamente exibida
#endif
}

/**
 * @brief Função auxiliar para limpar o buffer de entrada do teclado.
 * * Essencial após usar funções como 'scanf' ou 'atoi' que podem deixar o caractere de nova linha '\n'
 * no buffer, afetando leituras subsequentes com 'fgets' ou 'getchar'.
 */
void limparBufferEntrada() {
    int caractereLido;
    // Loop para ler e descartar caracteres do buffer até encontrar um '\n' ou o fim do arquivo (EOF).
    while ((caractereLido = getchar()) != '\n' && caractereLido != EOF);
}

/**
 * @brief Exibe as instruções de como jogar Torre de Hanói.
 * * O jogador deve pressionar Enter para retornar ao menu principal.
 */
void instrucoes() {
    clearScreen(); // Limpa a tela para exibir as instruções
    printf("        COMO JOGAR         \n");
    printf("================================\n\n");
    printf("Objetivo:\n");
    printf(" Mover todos os discos da haste inicial para a haste de destino.\n\n");
    printf("Como Jogar:\n");
    printf("  1. É possível mexer somente um disco por vez.\n");
    printf("  2. Só é possível colocar um disco sobre outro quando a base for maior.\n\n");
    printf("Pressione 'Enter' para voltar ao menu!\n");
    
    // getchar() aqui serve para pausar e esperar que o usuário pressione Enter.
    // Nenhuma limpeza de buffer extra é necessária antes, pois o menu principal já tratou.
    getchar(); 
}

/**
 * @brief Exibe o menu principal do jogo e gerencia as opções do usuário.
 * * Carrega o histórico de partidas no início e o salva ao sair.
 * * Gerencia a navegação entre as opções: instruções, jogar, histórico e sair.
 */
void exibirMenuPrincipal() {
    // Configura a localidade para português para garantir que caracteres especiais
    // (acentos, cedilha) sejam exibidos corretamente no console.
    setlocale(LC_ALL, "Portuguese"); 

    int opcaoMenuSelecionada;
    char termoBuscaJogador[50]; // Buffer para o nome do jogador na busca
    int numeroDiscosParaJogo = 0;

    // Tenta carregar as partidas salvas de um arquivo ao iniciar o programa.
    // Isso garante que o histórico seja persistente entre as execuções.
    carregarPartidasDeArquivo(NOME_ARQUIVO_HISTORICO);

    // Loop principal do menu, continua exibindo até que o usuário escolha sair (opção 0).
    do {
        clearScreen(); // Limpa a tela para apresentar o menu
        printf("        TORRE DE HANÓI          \n");
        printf("======================================\n\n");
        printf("Escolha uma opção:\n");
        printf("  1. Como Jogar\n");
        printf("  2. Iniciar Nova Partida\n"); // Renomeado para "Iniciar Nova Partida"
        printf("  3. Ver Histórico de Partidas\n"); // Renomeado para "Ver Histórico de Partidas"
        printf("  4. Buscar Partida no Histórico\n");
        printf("  0. Sair do Jogo\n\n");
        printf("Digite sua opção: ");

        char bufferEntrada[10]; // Buffer temporário para ler a entrada da opção
        // fgets lê a linha inteira, incluindo o '\n'. É mais seguro que scanf.
        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) == NULL) {
            opcaoMenuSelecionada = -1; // Sinaliza erro ou entrada inválida
        } else {
            // Converte a string lida para um inteiro.
            // atoi retorna 0 se a string não for um número válido.
            opcaoMenuSelecionada = atoi(bufferEntrada); 
        }
        
        // Estrutura de controle para as diferentes opções do menu
        switch (opcaoMenuSelecionada) {
            case 1:
                instrucoes(); // Chama a função que exibe as instruções
                break;
            case 2:
                clearScreen();
                printf("Digite seu nome (ate 49 caracteres): ");
                // fgets lê a linha inteira. O '\n' da entrada de 'opcaoMenuSelecionada'
                // já foi consumido pelo fgets anterior, então não precisamos limpar o buffer aqui.
                fgets(nomeJogadorAtual, sizeof(nomeJogadorAtual), stdin);
                // Remove o caractere de nova linha '\n' que fgets pode ter incluído.
                nomeJogadorAtual[strcspn(nomeJogadorAtual, "\n")] = '\0'; 

                printf("Quantos discos voce deseja jogar (min 3, max %d)? ", MAX_DISCOS_POSSIVEIS);
                // Novamente, fgets lê a linha completa, não precisa de limpeza prévia.
                if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) == NULL) {
                    numeroDiscosParaJogo = 0; // Sinaliza erro na leitura
                } else {
                    numeroDiscosParaJogo = atoi(bufferEntrada); // Converte para inteiro
                }
                
                // Validação do número de discos
                if (numeroDiscosParaJogo >= 3 && numeroDiscosParaJogo <= MAX_DISCOS_POSSIVEIS) {
                    jogar(numeroDiscosParaJogo); // Inicia a partida com o número de discos escolhido
                } else {
                    printf("\nNumero de discos invalido! Por favor, digite um valor entre 3 e %d.\n", MAX_DISCOS_POSSIVEIS);
                    printf("Pressione Enter para voltar ao menu principal...");
                    // Limpa o buffer caso o usuário digite algo que não seja apenas números e Enter.
                    limparBufferEntrada(); 
                    getchar(); // Espera o usuário pressionar Enter para continuar
                }
                break;
            case 3:
                clearScreen();
                exibirPartidasSalvas(); // Exibe o histórico de partidas
                printf("Pressione Enter para voltar ao menu principal...");
                limparBufferEntrada(); // Garante que o buffer esteja limpo antes de getchar
                getchar(); 
                break;
            case 4:
                clearScreen();
                printf("Digite o nome ou parte do nome do jogador para buscar: ");
                limparBufferEntrada(); // Limpa o '\n' que pode ter ficado da opção do menu
                fgets(termoBuscaJogador, sizeof(termoBuscaJogador), stdin);
                // Remove o '\n' que fgets adiciona no final da string.
                termoBuscaJogador[strcspn(termoBuscaJogador, "\n")] = '\0'; 
                buscarPartidasPorNome(termoBuscaJogador); // Realiza a busca no histórico
                printf("Pressione Enter para voltar ao menu principal...");
                limparBufferEntrada(); 
                getchar();
                break;
            case 0:
                clearScreen();
                printf("Obrigado por jogar! Salvando o historico e saindo...\n\n");
                salvarPartidasEmArquivo(NOME_ARQUIVO_HISTORICO); // Salva todas as partidas em arquivo
                liberarListaPartidasSalvas(); // Libera a memória alocada para o histórico
                break;
            default:
                printf("\nOpcao invalida! Por favor, escolha uma opcao listada.\n");
                printf("Pressione Enter para tentar novamente...");
                limparBufferEntrada(); 
                getchar(); 
                break;
        }

    } while (opcaoMenuSelecionada != 0); // Continua o loop enquanto a opção não for 'Sair'
}