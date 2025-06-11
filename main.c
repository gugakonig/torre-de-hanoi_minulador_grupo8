#include <stdio.h>    // Para funções de entrada/saída como printf, fgets
#include <stdlib.h>   // Para funções gerais como system, malloc, free
#include <locale.h>   // Para setlocale, permitindo caracteres especiais em português
#include <ctype.h>    // Para toupper, útil para converter letras para maiúsculas
#include <string.h>   // Para funções de string como strlen, strcspn

// Inclusão dos cabeçalhos das outras partes do projeto
#include "pilha.h"     // Contém as definições e protótipos para a estrutura da pilha
#include "historico.h" // Contém as definições e protótipos para o histórico de partidas
#include "menu.h"      // Contém as definições e protótipos para o menu principal e funções auxiliares

// Constantes globais
#define NUMERO_DE_PINOS 3          // Número fixo de pinos na Torre de Hanói (3 pinos: A, B, C).

// Acessa a variável global nomeJogadorAtual, que é definida em menu.c.
extern char nomeJogadorAtual[50]; 

// Declaração de função auxiliar para limpar o buffer de entrada.
// Sua implementação está em menu.c, mas precisamos declará-la aqui para usar.
extern void limparBufferEntrada(); 

/**
 * @brief Imprime a representação visual de um disco ou da haste (se tamanho 0).
 * * Esta função calcula o espaçamento necessário para centralizar o disco
 * (ou a haste vertical) na largura máxima de um pino.
 * * @param tamanho O tamanho do disco (1 para o menor, etc.). 0 representa a haste vertical.
 * @param larguraMax A largura máxima que um pino pode ocupar no display, baseada no maior disco.
 */
void imprimirDisco(int tamanho, int larguraMax) {
    // A largura visual de um disco é (2 * tamanho - 1). Ex: disco 1 = 1, disco 2 = 3, disco 3 = 5.
    // O tamanho 0 (representando a haste) tem largura visual de 1.
    int larguraVisualDisco = (tamanho == 0) ? 1 : (2 * tamanho - 1);
    
    // Calcula o número de espaços para centralizar o disco no pino.
    // Divide por 2 para ter espaços iguais em ambos os lados.
    int espacosLaterais = (larguraMax - larguraVisualDisco) / 2;

    // Imprime os espaços à esquerda
    for (int i = 0; i < espacosLaterais; i++) {
        printf(" ");
    }
    
    // Imprime o disco ou a haste
    if (tamanho == 0) {
        printf("|"); // Representa a parte vertical da haste
    } else {
        for (int i = 0; i < larguraVisualDisco; i++) {
            printf("="); // Representa um disco
        }
    }
    
    // Imprime os espaços à direita
    for (int i = 0; i < espacosLaterais; i++) {
        printf(" ");
    }
}

/**
 * @brief Exibe o estado atual de todas as torres no console.
 * * Percorre os pinos de cima para baixo (do disco menor para o maior),
 * imprimindo os discos em suas respectivas posições. Se não houver disco
 * em uma determinada altura, imprime o espaço vazio ou a haste.
 * * @param pinos Array de ponteiros para Pilha, representando as três torres.
 * @param totalDiscosJogo O número total de discos usados nesta partida.
 */
void exibirTorres(Pilha* pinos[], int totalDiscosJogo) {
    clearScreen(); // Limpa a tela antes de redesenhar as torres
    printf("\nTorre de Hanoi - %d discos\n\n", totalDiscosJogo);

    // A largura máxima de um pino é determinada pelo maior disco (totalDiscosJogo)
    // que tem largura visual de (2 * totalDiscosJogo - 1).
    int larguraMaximaPino = (2 * totalDiscosJogo - 1);

    // Loop para exibir os níveis dos pinos, do topo para a base
    for (int nivelAtual = totalDiscosJogo - 1; nivelAtual >= 0; nivelAtual--) {
        for (int i = 0; i < NUMERO_DE_PINOS; i++) {
            int discoAExibir = 0; // Se for 0, imprime a haste ou espaço vazio

            // Calcula a altura atual de cada pilha (quantos discos tem)
            int alturaPinoAtual = 0;
            No* noContador = pinos[i]->topo;
            while(noContador != NULL) {
                alturaPinoAtual++;
                noContador = noContador->abaixo;
            }

            // Verifica se há um disco neste nível da pilha (baseado na altura)
            if (nivelAtual < alturaPinoAtual) {
                No* noAtual = pinos[i]->topo;
                // Percorre a pilha até encontrar o disco no 'nivelAtual' (de baixo para cima)
                for (int d = 0; d < (alturaPinoAtual - 1 - nivelAtual) && noAtual != NULL; d++) {
                    noAtual = noAtual->abaixo;
                }
                // Se encontrou, pega o tamanho do disco
                if (noAtual != NULL) {
                    discoAExibir = noAtual->tamanhoDisco;
                }
            }
            
            imprimirDisco(discoAExibir, larguraMaximaPino);
            printf("   "); // Espaço entre os pinos
        }
        printf("\n"); // Nova linha para o próximo nível
    }

    // Imprime a base dos pinos (os traços "---")
    for (int i = 0; i < NUMERO_DE_PINOS; i++) {
        int larguraBase = larguraMaximaPino;
        // Centraliza a base "---"
        for (int j = 0; j < larguraBase / 2; j++) printf(" ");
        printf("---"); 
        for (int j = 0; j < larguraBase / 2; j++) printf(" ");
        printf("   ");
    }
    printf("\n");

    // Imprime os nomes dos pinos (A, B, C)
    for (int i = 0; i < NUMERO_DE_PINOS; i++) {
        int larguraNome = larguraMaximaPino;
        // Centraliza o nome do pino
        for (int j = 0; j < larguraNome / 2; j++) printf(" ");
        printf("%c", pinos[i]->nome); // Nome da haste (A, B ou C)
        for (int j = 0; j < larguraNome / 2; j++) printf(" ");
        printf("   ");
    }
    printf("\n");
}

/**
 * @brief Verifica se todos os discos estão na torre de destino na ordem correta.
 * * No jogo da Torre de Hanói, o objetivo é ter todos os discos na torre final
 * empilhados em ordem crescente (do menor para o maior, de cima para baixo).
 * * @param torre Ponteiro para a Pilha que representa a torre de destino.
 * @param numeroTotalDeDiscos O número total de discos na partida.
 * @return 1 se a torre está completa e na ordem correta, 0 caso contrário.
 */
int verificarOrdemDiscos(Pilha* torre, int numeroTotalDeDiscos) {
    // Se a torre está vazia, ela só pode ser considerada "completa e correta"
    // se o número esperado de discos for 0 (o que não acontece na vitória).
    if (pilhaVazia(torre)) {
        return (numeroTotalDeDiscos == 0);
    }

    No* noAtual = torre->topo; // Começa pelo topo da pilha (menor disco)
    int discosContados = 0;
    int proximoTamanhoEsperado = 1; // O menor disco é o de tamanho 1

    // Percorre a pilha, verificando a ordem dos discos
    while (noAtual != NULL) {
        // Se o tamanho do disco atual não corresponder ao esperado, a ordem está incorreta
        if (noAtual->tamanhoDisco != proximoTamanhoEsperado) {
            return 0; // Ordem inválida
        }
        proximoTamanhoEsperado++; // Espera o próximo disco maior
        discosContados++;         // Conta os discos empilhados
        noAtual = noAtual->abaixo; // Move para o próximo disco na pilha
    }
    // Retorna verdadeiro se o número de discos contados na torre é igual ao total de discos do jogo
    return (discosContados == numeroTotalDeDiscos);
}

/**
 * @brief Converte uma letra de pino ('A', 'B', 'C') para seu índice numérico (0, 1, 2).
 * * @param letra A letra do pino (maiúscula ou minúscula).
 * @return O índice do pino (0 para A, 1 para B, 2 para C), ou -1 se a letra for inválida.
 */
int obterIndiceDoPino(char letra) {
    letra = toupper(letra); // Converte a letra para maiúscula para padronização
    if (letra == 'A') return 0;
    if (letra == 'B') return 1;
    if (letra == 'C') return 2;
    return -1; // Retorna -1 para letras inválidas
}

/**
 * @brief Implementa a lógica principal do jogo Torre de Hanói.
 * * Gerencia o estado dos pinos, a interação do jogador, a contagem de movimentos
 * e a verificação das condições de vitória e saída.
 * * @param numDiscos O número de discos para a partida atual.
 */
void jogar(int numDiscos) {
    // Array para armazenar os ponteiros para as três pilhas (pinos A, B, C)
    Pilha* pinosDoJogo[NUMERO_DE_PINOS];
    
    // Cria um objeto HistoricoMovimentos para armazenar o número total de movimentos da partida.
    // Não armazena detalhes de cada movimento para otimização e simplicidade de gravação.
    HistoricoMovimentos* historicoPartida = criarHistoricoMovimentos(); 
    int contadorMovimentos = 0; // Inicializa o contador de movimentos para esta partida

    // Verifica se a alocação de memória para o histórico foi bem-sucedida.
    if (historicoPartida == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel alocar memoria para o historico da partida.\n");
        return; // Aborta a função se não conseguir criar o histórico
    }

    // Inicializa os três pinos (torres)
    pinosDoJogo[0] = criarPilha('A'); // Pino A
    pinosDoJogo[1] = criarPilha('B'); // Pino B
    pinosDoJogo[2] = criarPilha('C'); // Pino C

    // Verifica se a alocação de memória para os pinos foi bem-sucedida.
    // Se falhar, libera o que já foi alocado e retorna.
    if (pinosDoJogo[0] == NULL || pinosDoJogo[1] == NULL || pinosDoJogo[2] == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel criar uma ou mais pilhas para o jogo.\n");
        for (int i = 0; i < NUMERO_DE_PINOS; i++) {
            if (pinosDoJogo[i] != NULL) liberarPilha(pinosDoJogo[i]);
        }
        liberarHistoricoMovimentos(historicoPartida); // Libera o histórico alocado
        return;
    }

    // Empilha todos os discos no primeiro pino (Pino A) em ordem decrescente (maior embaixo)
    for (int i = numDiscos; i >= 1; i--) {
        empilhar(pinosDoJogo[0], i);
    }

    char letraOrigem, letraDestino; // Caracteres para as letras dos pinos de origem e destino
    int indiceOrigem, indiceDestino; // Índices numéricos correspondentes aos pinos
    int discoSendoMovido;           // Armazena o tamanho do disco que está sendo movido

    // Loop principal do jogo
    while (1) {
        exibirTorres(pinosDoJogo, numDiscos); // Atualiza e exibe o estado das torres
        printf("Numero de movimentos: %d\n", contadorMovimentos); // Exibe a contagem de movimentos

        // Condição de vitória: Todos os discos no pino C e na ordem correta
        if (pilhaVazia(pinosDoJogo[0]) && pilhaVazia(pinosDoJogo[1]) && verificarOrdemDiscos(pinosDoJogo[2], numDiscos)) {
            printf("\nParabéns, %s! Você concluiu o jogo com %d movimentos!\n", nomeJogadorAtual, contadorMovimentos);

            // Antes de adicionar ao histórico, atualizamos o número de movimentos no objeto historicoPartida
            historicoPartida->numMovimentos = contadorMovimentos; 
            adicionarPartida(nomeJogadorAtual, numDiscos, historicoPartida); // Registra o resumo da partida no histórico global
            
            // Libera a memória alocada para as pilhas do jogo
            for (int i = 0; i < NUMERO_DE_PINOS; i++) {
                liberarPilha(pinosDoJogo[i]);
            }

            printf("Pressione Enter para voltar ao menu...");
            limparBufferEntrada(); // Garante que o buffer de entrada está limpo antes do getchar()
            getchar(); // Espera a confirmação do jogador
            break; // Sai do loop principal do jogo
        }

        printf("\nDigite seu movimento (ex: AB para mover de A para B), 'R' para reiniciar, 'Q' para sair: ");
        
        char entradaDoJogador[10]; // Buffer para ler a entrada do jogador
        // fgets lê a linha inteira, incluindo o '\n'. Não precisa de limpeza antes.
        if (fgets(entradaDoJogador, sizeof(entradaDoJogador), stdin) == NULL) {
            continue; // Se a leitura falhar, tenta novamente
        }
        // Remove o caractere de nova linha '\n' que fgets adiciona.
        entradaDoJogador[strcspn(entradaDoJogador, "\n")] = '\0'; 

        // Processa a entrada do jogador com base no seu comprimento
        if (strlen(entradaDoJogador) == 1) {
            letraOrigem = toupper(entradaDoJogador[0]); // Converte para maiúscula para comparação

            // Opção para Sair do jogo
            if (letraOrigem == 'Q') {
                printf("Saindo do jogo atual...\n");
                liberarHistoricoMovimentos(historicoPartida); // Libera a memória do histórico desta partida
                // Libera a memória das pilhas antes de sair
                for (int i = 0; i < NUMERO_DE_PINOS; i++) {
                    liberarPilha(pinosDoJogo[i]);
                }
                break; // Sai do loop principal do jogo
            }
            // Opção para Reiniciar o jogo
            if (letraOrigem == 'R') {
                printf("Reiniciando jogo...\n");
                liberarHistoricoMovimentos(historicoPartida); // Libera o histórico da partida atual
                // Libera as pilhas antes de reiniciar
                for (int i = 0; i < NUMERO_DE_PINOS; i++) {
                    liberarPilha(pinosDoJogo[i]);
                }
                jogar(numDiscos); // Chama a função jogar recursivamente para iniciar uma nova partida
                return; // Retorna da chamada atual de jogar para evitar continuar o loop anterior
            }
        } 
        // Entrada de movimento (ex: "AB")
        else if (strlen(entradaDoJogador) == 2) {
            letraOrigem = toupper(entradaDoJogador[0]);
            letraDestino = toupper(entradaDoJogador[1]);
        } 
        // Entrada inválida
        else {
            printf("Entrada invalida! Digite 2 letras (ex: AB) ou 'R'/'Q'. Pressione Enter para continuar...");
            limparBufferEntrada(); // Limpa o buffer caso haja caracteres extras
            getchar(); // Espera a confirmação do jogador
            continue; // Volta ao início do loop para nova entrada
        }

        // Converte as letras dos pinos para seus respectivos índices
        indiceOrigem = obterIndiceDoPino(letraOrigem);
        indiceDestino = obterIndiceDoPino(letraDestino);

        // Validação do movimento
        // 1. Índices de pino válidos (0, 1, 2)
        // 2. Pino de origem não está vazio
        // 3. Se o pino de destino não está vazio, o disco a ser movido deve ser menor que o disco no topo do destino.
        if (indiceOrigem == -1 || indiceDestino == -1 ||
            pilhaVazia(pinosDoJogo[indiceOrigem]) ||
            (topoDisco(pinosDoJogo[indiceDestino]) != -1 && topoDisco(pinosDoJogo[indiceDestino]) < topoDisco(pinosDoJogo[indiceOrigem]))) {
            
            printf("Movimento invalido! Pressione Enter para continuar...");
            limparBufferEntrada(); // Limpa o buffer de entrada
            getchar(); // Espera a confirmação do jogador
            continue; // Volta ao início do loop para nova entrada
        }

        // Executa o movimento válido
        discoSendoMovido = desempilhar(pinosDoJogo[indiceOrigem]); // Tira o disco do pino de origem
        empilhar(pinosDoJogo[indiceDestino], discoSendoMovido);   // Coloca o disco no pino de destino
        contadorMovimentos++; // Incrementa o contador de movimentos
        // A função registrarMovimento para detalhes não é mais chamada aqui,
        // pois estamos apenas contando os movimentos para o resumo da partida.
    }
}

/**
 * @brief Função principal do programa.
 * * Configura a localidade para português e inicia o menu principal do jogo.
 * * @return 0 se o programa executar com sucesso.
 */
int main() {
    setlocale(LC_ALL, "Portuguese"); // Define a localidade para português para usar caracteres especiais
    exibirMenuPrincipal();           // Chama a função que exibe o menu e gerencia o fluxo do jogo
    return 0; // Indica que o programa terminou com sucesso
}