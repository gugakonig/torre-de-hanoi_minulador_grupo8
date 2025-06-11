#ifndef HISTORICO_H
#define HISTORICO_H

// Estrutura para armazenar o resumo de uma partida
typedef struct {
    char nomeJogador[50];    // Nome do jogador que jogou a partida
    int numDiscos;           // Número de discos usados nessa partida
    int numMovimentos;       // Total de movimentos feitos para completar a partida
} Partida;

// Estrutura para gerenciar o histórico de movimentos de uma única partida
// (usada temporariamente durante o jogo para contar movimentos)
typedef struct {
    int numMovimentos;      // Total de movimentos para a partida atual
    // Não precisamos de uma lista de movimentos individuais aqui,
    // pois o objetivo é apenas armazenar o total para a Partida.
} HistoricoMovimentos;

// Nó da lista encadeada para armazenar partidas no histórico GERAL
typedef struct NoHistorico {
    Partida partida;
    struct NoHistorico *proximo;
} NoHistorico;

// Estrutura do histórico global de partidas (lista encadeada)
typedef struct {
    NoHistorico *inicio;
} HistoricoGlobal;

// Variável global para o histórico, acessível por outras partes do programa
extern HistoricoGlobal *historicoGlobal; 

// Protótipos das funções
void inicializarHistoricoGlobal();
HistoricoMovimentos* criarHistoricoMovimentos();
void liberarHistoricoMovimentos(HistoricoMovimentos* historico);
void adicionarPartida(const char* nomeJogador, int numDiscos, HistoricoMovimentos* historicoPartida);
void exibirHistorico();
void salvarHistoricoEmArquivo(const char* nomeArquivo);
void carregarHistoricoDeArquivo(const char* nomeArquivo);
void liberarHistoricoGlobal();

#endif // HISTORICO_H