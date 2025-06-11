#include <stdio.h>    // Para funções de entrada/saída de arquivo e console
#include <stdlib.h>   // Para funções de alocação de memória (malloc, free)
#include <string.h>   // Para funções de manipulação de strings (strncpy, strstr, strlen, strcspn)
#include "historico.h" // Inclui o cabeçalho com as definições das estruturas e protótipos

// Variável global que aponta para o início da lista encadeada de partidas salvas.
// 'NULL' indica que a lista está vazia.
Partida* listaPartidasSalvas = NULL;

/**
 * @brief Aloca e inicializa uma nova estrutura HistoricoMovimentos.
 * * Esta estrutura é usada temporariamente durante uma partida ativa para contar
 * o total de movimentos, mas não persiste os movimentos individuais.
 * @return Um ponteiro para a nova estrutura HistoricoMovimentos, ou NULL se a alocação falhar.
 */
HistoricoMovimentos* criarHistoricoMovimentos() {
    HistoricoMovimentos* novoHistorico = (HistoricoMovimentos*)malloc(sizeof(HistoricoMovimentos));
    if (novoHistorico != NULL) {
        novoHistorico->inicio = NULL;         // Lista de movimentos vazia no início
        novoHistorico->fim = NULL;            // Ponteiro para o fim também nulo
        novoHistorico->numMovimentos = 0;    // Contador de movimentos inicializado em zero
    } else {
        // Em caso de falha na alocação, uma mensagem de erro pode ser útil para depuração.
        fprintf(stderr, "Erro de alocacao de memoria para HistoricoMovimentos.\n");
    }
    return novoHistorico;
}

/**
 * @brief Registra um movimento individual em um histórico temporário.
 * * NOTA: Esta função não é mais utilizada para persistir dados,
 * apenas para demonstração ou caso os movimentos detalhados precisem ser rastreados em tempo de execução.
 * O foco agora é apenas no 'numMovimentos' da estrutura HistoricoMovimentos.
 * @param historico O ponteiro para a estrutura HistoricoMovimentos da partida atual.
 * @param disco O tamanho do disco que foi movido.
 * @param origem O pino de origem do movimento ('A', 'B', 'C').
 * @param destino O pino de destino do movimento ('A', 'B', 'C').
 */
void registrarMovimento(HistoricoMovimentos* historico, int disco, char origem, char destino) {
    if (historico == NULL) {
        fprintf(stderr, "Erro: HistoricoMovimentos eh NULL ao tentar registrar movimento.\n");
        return; // Não é possível registrar em um histórico nulo
    }

    Movimento* novoRegistro = (Movimento*)malloc(sizeof(Movimento));
    if (novoRegistro != NULL) {
        novoRegistro->disco = disco;
        // CORREÇÃO: Nomes de campo atualizados para 'pinoOrigem' e 'pinoDestino'
        novoRegistro->pinoOrigem = origem;
        novoRegistro->pinoDestino = destino;
        novoRegistro->prox = NULL; // Novo movimento sempre será o último por enquanto

        if (historico->inicio == NULL) {
            historico->inicio = novoRegistro; // Primeiro movimento na lista
        } else {
            historico->fim->prox = novoRegistro; // Adiciona ao final da lista
        }
        historico->fim = novoRegistro; // Atualiza o ponteiro para o último movimento
        historico->numMovimentos++;    // Incrementa o contador total de movimentos
    } else {
        fprintf(stderr, "Erro de alocacao de memoria para Movimento.\n");
    }
}

/**
 * @brief Libera a memória alocada para uma estrutura HistoricoMovimentos e seus movimentos internos.
 * * Usado para limpar o histórico temporário de uma partida após ela ser concluída ou abandonada.
 * @param historico O ponteiro para a estrutura HistoricoMovimentos a ser liberada.
 */
void liberarHistoricoMovimentos(HistoricoMovimentos* historico) {
    if (historico == NULL) return; // Nada para liberar

    Movimento* movimentoAtual = historico->inicio;
    Movimento* proximoMovimento;
    while (movimentoAtual != NULL) {
        proximoMovimento = movimentoAtual->prox; // Guarda o próximo antes de liberar o atual
        free(movimentoAtual);                  // Libera a memória do nó do movimento
        movimentoAtual = proximoMovimento;     // Avança para o próximo
    }
    free(historico); // Libera a própria estrutura HistoricoMovimentos
}

/**
 * @brief Adiciona os dados resumidos de uma partida à lista global de partidas salvas.
 * * Esta função aloca uma nova estrutura Partida e a preenche com as informações
 * de nome do jogador, número de discos e total de movimentos.
 * * A estrutura HistoricoMovimentos detalhada da partida (movimentos individuais)
 * é liberada após a extração do 'numMovimentos', pois não é persistida.
 * @param nomeDoJogador O nome do jogador que jogou a partida.
 * @param numeroDeDiscos O número de discos utilizados na partida.
 * @param historicoDaPartida O ponteiro para a estrutura HistoricoMovimentos temporária da partida.
 */
void adicionarPartida(const char* nomeDoJogador, int numeroDeDiscos, HistoricoMovimentos* historicoDaPartida) {
    Partida* novaPartida = (Partida*)malloc(sizeof(Partida));
    if (novaPartida == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para nova Partida.\n");
        // Se a alocação da partida falhar, liberamos o histórico de movimentos
        // para evitar vazamento de memória, já que ele não será adicionado.
        liberarHistoricoMovimentos(historicoDaPartida); 
        return;
    }

    // Copia o nome do jogador, garantindo que não exceda o buffer e seja nulo-terminado.
    strncpy(novaPartida->nomeJogador, nomeDoJogador, sizeof(novaPartida->nomeJogador) - 1);
    novaPartida->nomeJogador[sizeof(novaPartida->nomeJogador) - 1] = '\0'; // Garante terminação nula

    novaPartida->numDiscos = numeroDeDiscos;
    // O número total de movimentos é obtido do histórico temporário da partida.
    novaPartida->totalMovimentos = historicoDaPartida->numMovimentos;
    
    // O ponteiro 'movimentosPartida' na struct Partida salva será NULL,
    // pois os detalhes individuais dos movimentos não são armazenados.
    novaPartida->movimentosPartida = NULL; 
    
    // Libera a memória do histórico de movimentos que foi criado para a partida atual,
    // pois ele já serviu ao seu propósito de contar os movimentos e não será persistido.
    liberarHistoricoMovimentos(historicoDaPartida); 

    novaPartida->prox = NULL; // Nova partida sempre adicionada ao final

    // Adiciona a nova partida à lista encadeada global
    if (listaPartidasSalvas == NULL) {
        listaPartidasSalvas = novaPartida; // É a primeira partida na lista
    } else {
        Partida* partidaAtual = listaPartidasSalvas;
        while (partidaAtual->prox != NULL) {
            partidaAtual = partidaAtual->prox; // Percorre a lista até o final
        }
        partidaAtual->prox = novaPartida; // Adiciona a nova partida ao final
    }
}

/**
 * @brief Exibe todas as partidas salvas no histórico em um formato legível.
 * * Percorre a lista encadeada de partidas salvas e imprime os detalhes de cada uma.
 */
void exibirPartidasSalvas() {
    if (listaPartidasSalvas == NULL) {
        printf("Nenhuma partida salva ainda.\n");
        return;
    }

    printf("\n--- HISTÓRICO DE PARTIDAS ---\n");
    Partida* partidaAtual = listaPartidasSalvas;
    int contador = 1; // Para numerar as partidas exibidas
    while (partidaAtual != NULL) {
        printf("Partida %d: Jogador: %s | Discos: %d | Movimentos: %d\n",
               contador++, partidaAtual->nomeJogador, partidaAtual->numDiscos, partidaAtual->totalMovimentos);
        printf("-----------------------------\n");
        partidaAtual = partidaAtual->prox; // Avança para a próxima partida na lista
    }
}

/**
 * @brief Busca e exibe partidas salvas cujo nome do jogador contenha a string de busca.
 * * A busca é case-sensitive (sensível a maiúsculas/minúsculas).
 * @param termoDeBusca O nome ou parte do nome do jogador a ser buscado.
 */
void buscarPartidasPorNome(const char* termoDeBusca) {
    if (listaPartidasSalvas == NULL) {
        printf("Nenhuma partida salva para buscar.\n");
        return;
    }

    printf("\n--- RESULTADOS DA BUSCA por '%s' ---\n", termoDeBusca);
    Partida* partidaAtual = listaPartidasSalvas;
    int partidasEncontradas = 0;
    int contador = 1;
    while (partidaAtual != NULL) {
        // strstr busca uma substring dentro de outra. Se encontrar, retorna um ponteiro não-NULL.
        if (strstr(partidaAtual->nomeJogador, termoDeBusca) != NULL) {
            printf("Partida %d: Jogador: %s | Discos: %d | Movimentos: %d\n",
                   contador++, partidaAtual->nomeJogador, partidaAtual->numDiscos, partidaAtual->totalMovimentos);
            printf("-----------------------------\n");
            partidasEncontradas++;
        }
        partidaAtual = partidaAtual->prox;
    }

    if (partidasEncontradas == 0) {
        printf("Nenhuma partida encontrada para '%s'.\n", termoDeBusca);
    }
}

/**
 * @brief Salva todas as partidas da lista global em um arquivo de texto.
 * * Cada partida é salva em uma linha no formato "Jogador: Nome | Discos: N | Movimentos: M".
 * @param nomeDoArquivo O nome do arquivo para salvar.
 */
void salvarPartidasEmArquivo(const char* nomeDoArquivo) {
    // Abre o arquivo no modo de escrita ("w"). Se o arquivo não existir, ele é criado.
    // Se existir, seu conteúdo anterior é sobrescrito.
    FILE* arquivoDeHistorico = fopen(nomeDoArquivo, "w");
    if (arquivoDeHistorico == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo '%s' para escrita. Verifique permissoes.\n", nomeDoArquivo);
        return;
    }

    Partida* partidaAtual = listaPartidasSalvas;
    while (partidaAtual != NULL) {
        // Escreve os dados da partida no arquivo no formato especificado.
        fprintf(arquivoDeHistorico, "Jogador: %s | Discos: %d | Movimentos: %d\n",
                partidaAtual->nomeJogador,
                partidaAtual->numDiscos,
                partidaAtual->totalMovimentos);
        
        partidaAtual = partidaAtual->prox; // Avança para a próxima partida
    }

    fclose(arquivoDeHistorico); // Fecha o arquivo para salvar as alterações
}

/**
 * @brief Carrega partidas de um arquivo de texto para a lista global de partidas salvas.
 * * Limpa o histórico atual antes de carregar.
 * * Lê cada linha do arquivo, parseia os dados e os adiciona à lista.
 * @param nomeDoArquivo O nome do arquivo de onde as partidas serão carregadas.
 */
void carregarPartidasDeArquivo(const char* nomeDoArquivo) {
    liberarListaPartidasSalvas(); // Limpa a lista atual para evitar duplicatas ou dados antigos

    // Abre o arquivo no modo de leitura ("r").
    FILE* arquivoDeHistorico = fopen(nomeDoArquivo, "r");
    if (arquivoDeHistorico == NULL) {
        // Se o arquivo não existir ou não puder ser aberto, isso é normal na primeira execução.
        // fprintf(stderr, "Aviso: Arquivo de historico '%s' nao encontrado. Sera criado um novo na proxima gravacao.\n", nomeDoArquivo);
        return;
    }

    char linhaDoArquivo[256]; // Buffer para ler uma linha completa do arquivo
    char nomeDoJogadorCarregado[50];
    int numDiscosCarregados, totalMovimentosCarregados;
    
    // Lê o arquivo linha por linha
    while (fgets(linhaDoArquivo, sizeof(linhaDoArquivo), arquivoDeHistorico) != NULL) {
        // Remove o caractere de nova linha '\n' lido por fgets, se presente.
        linhaDoArquivo[strcspn(linhaDoArquivo, "\n")] = '\0'; 

        // Ignora linhas vazias que podem existir no arquivo.
        if (strlen(linhaDoArquivo) == 0) {
            continue;
        }

        // Tenta parsear a linha usando sscanf no formato esperado.
        // %49[^|] lê até 49 caracteres que não sejam '|' para o nome do jogador.
        if (sscanf(linhaDoArquivo, "Jogador: %49[^|] | Discos: %d | Movimentos: %d",
                   nomeDoJogadorCarregado, &numDiscosCarregados, &totalMovimentosCarregados) == 3) {
            
            // Remove espaços em branco extras no final do nome do jogador, se houver.
            size_t comprimentoNome = strlen(nomeDoJogadorCarregado);
            while (comprimentoNome > 0 && nomeDoJogadorCarregado[comprimentoNome - 1] == ' ') {
                nomeDoJogadorCarregado[--comprimentoNome] = '\0';
            }

            // Cria um "historico dummy" para a função adicionarPartida.
            // Ele só precisa ter o 'numMovimentos' correto; os detalhes são ignorados.
            HistoricoMovimentos* historicoDummy = criarHistoricoMovimentos();
            if (historicoDummy == NULL) {
                fprintf(stderr, "Erro interno ao alocar historicoDummy para carregamento.\n");
                continue; // Pula esta partida e tenta a próxima
            }
            historicoDummy->numMovimentos = totalMovimentosCarregados; // Define os movimentos lidos
            
            // Adiciona a partida carregada à lista global.
            // A função 'adicionarPartida' irá liberar o historicoDummy.
            adicionarPartida(nomeDoJogadorCarregado, numDiscosCarregados, historicoDummy); 
        } else {
            // Se o formato da linha não corresponder, imprime um aviso (útil para depuração).
            fprintf(stderr, "Aviso: Formato de linha inesperado no historico.txt: '%s'. Ignorando.\n", linhaDoArquivo);
        }
    }

    fclose(arquivoDeHistorico); // Fecha o arquivo
}

/**
 * @brief Libera toda a memória alocada para a lista global de partidas salvas.
 * * Percorre a lista encadeada e libera cada nó da Partida.
 * * Deve ser chamada ao final do programa para evitar vazamentos de memória.
 */
void liberarListaPartidasSalvas() {
    Partida* partidaAtual = listaPartidasSalvas;
    Partida* proximaPartida;
    while (partidaAtual != NULL) {
        proximaPartida = partidaAtual->prox; // Guarda o ponteiro para o próximo antes de liberar o atual
        // O campo 'movimentosPartida' já é NULL para partidas carregadas/salvas,
        // então não precisamos chamar liberarHistoricoMovimentos aqui novamente.
        free(partidaAtual); // Libera a estrutura da partida
        partidaAtual = proximaPartida; // Avança para a próxima partida
    }
    listaPartidasSalvas = NULL; // Garante que a lista global esteja nula após a liberação
}