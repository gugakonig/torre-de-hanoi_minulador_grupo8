#ifndef HISTORICO_H
#define HISTORICO_H

// Este cabeçalho define as estruturas de dados e protótipos de funções
// relacionadas ao gerenciamento do histórico de partidas da Torre de Hanói.
// Ele permite que outros módulos (como main.c e menu.c) interajam com
// o sistema de histórico sem precisar conhecer os detalhes de implementação.

// Constante para o nome do arquivo do histórico, relevante apenas para o módulo de histórico.
#define NOME_ARQUIVO_HISTORICO "historico.txt" 

/**
 * @brief Define a estrutura para um movimento individual.
 * * Cada nó desta lista representa um único movimento de um disco entre pinos.
 * * Embora não seja persistido em arquivo (por simplicidade), é fundamental
 * para o rastreamento em tempo de execução dentro da lógica do jogo, se necessário.
 */
typedef struct Movimento {
    int disco;         // O tamanho do disco movido (ex: 1 para o menor, 2 para o próximo)
    char pinoOrigem;   // O pino de onde o disco foi movido (ex: 'A', 'B', 'C')
    char pinoDestino;  // O pino para onde o disco foi movido (ex: 'A', 'B', 'C')
    struct Movimento* prox; // Ponteiro para o próximo movimento na lista encadeada
} Movimento;

/**
 * @brief Define a estrutura para o histórico detalhado de movimentos de uma única partida.
 * * Contém uma lista encadeada de todos os movimentos e o número total deles.
 * * Esta estrutura é utilizada durante a execução da partida para acumular movimentos
 * e calcular o total ao final. É uma estrutura temporária que é liberada após
 * os dados resumidos serem transferidos para a 'Partida' salva.
 */
typedef struct HistoricoMovimentos {
    Movimento* inicio;      // Ponteiro para o primeiro movimento da partida
    Movimento* fim;         // Ponteiro para o fim da lista de movimentos (para adição eficiente)
    int numMovimentos;      // Contador do número total de movimentos nesta partida
} HistoricoMovimentos;

/**
 * @brief Define a estrutura para uma partida salva no histórico geral.
 * * Armazena um resumo dos dados de uma partida concluída para exibição e salvamento em arquivo.
 * * NOTA: Não armazena os movimentos individuais da partida para economizar memória e simplificar I/O.
 */
typedef struct Partida {
    char nomeJogador[50];       // Nome do jogador que completou a partida
    int numDiscos;              // Número de discos usados na partida
    int totalMovimentos;        // Número total de movimentos que o jogador fez para vencer
    
    // O ponteiro 'movimentosPartida' aqui será NULL para partidas carregadas/salvas.
    // Ele é mantido na struct Partida APENAS para compatibilidade com a assinatura da função adicionarPartida()
    // que recebe um HistoricoMovimentos* para extrair o totalMovimentos e liberá-lo.
    HistoricoMovimentos* movimentosPartida; 

    struct Partida* prox;       // Ponteiro para a próxima partida na lista encadeada global
} Partida;

// Variável global (declarada como 'extern') que aponta para o início
// da lista encadeada de todas as partidas salvas.
// Sua definição (alocação de memória) está em historico.c.
extern Partida* listaPartidasSalvas;

// --- Protótipos das Funções de Gerenciamento do Histórico ---

/**
 * @brief Cria e inicializa uma nova estrutura HistoricoMovimentos temporária.
 * @return Um ponteiro para a nova estrutura HistoricoMovimentos.
 */
HistoricoMovimentos* criarHistoricoMovimentos();

/**
 * @brief Registra um movimento individual dentro de um HistoricoMovimentos.
 * * Este método é usado apenas para a contagem de movimentos durante a partida,
 * não para salvamento detalhado.
 * @param historico O histórico temporário da partida.
 * @param disco O tamanho do disco movido.
 * @param origem O pino de origem.
 * @param destino O pino de destino.
 */
void registrarMovimento(HistoricoMovimentos* historico, int disco, char origem, char destino);

/**
 * @brief Libera toda a memória alocada para um HistoricoMovimentos específico.
 * @param historico O histórico a ser liberado.
 */
void liberarHistoricoMovimentos(HistoricoMovimentos* historico);

/**
 * @brief Adiciona os dados resumidos de uma partida completa à lista global de partidas salvas.
 * * Libera o HistoricoMovimentos original após a adição.
 * @param nomeJogador O nome do jogador.
 * @param numDiscos O número de discos da partida.
 * @param movimentosPartida O histórico temporário da partida (será liberado por esta função).
 */
void adicionarPartida(const char* nomeJogador, int numDiscos, HistoricoMovimentos* movimentosPartida);

/**
 * @brief Exibe todas as partidas atualmente carregadas na memória.
 */
void exibirPartidasSalvas();

/**
 * @brief Busca e exibe partidas no histórico pelo nome do jogador.
 * @param nomeBusca O nome ou parte do nome a ser pesquisado.
 */
void buscarPartidasPorNome(const char* nomeBusca);

/**
 * @brief Salva a lista de partidas salvas em um arquivo de texto.
 * @param nomeArquivo O nome do arquivo para salvar.
 */
void salvarPartidasEmArquivo(const char* nomeArquivo);

/**
 * @brief Carrega partidas de um arquivo de texto para a memória.
 * * Limpa o histórico atual antes de carregar.
 * @param nomeArquivo O nome do arquivo para carregar.
 */
void carregarPartidasDeArquivo(const char* nomeArquivo);

/**
 * @brief Libera toda a memória alocada para a lista global de partidas salvas.
 * * Essencial para evitar vazamentos de memória ao final do programa.
 */
void liberarListaPartidasSalvas();

#endif // HISTORICO_H