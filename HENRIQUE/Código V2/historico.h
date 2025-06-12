#ifndef HISTORICO_H
#define HISTORICO_H

// Estrutura para o histórico (nó da lista encadeada)
typedef struct Historico {
    char nomeJogador[50];
    char data[11];
    int modoJogo; // Número de discos
    int movimentos;
    struct Historico *proximo;
} Historico;

// Declarações das Funções de Histórico
void adicionarHistoricoLista(Historico **lista, const char *nome, const char *data, int modo, int movimentos);
void salvarHistoricoArquivo(const char *nome, const char *data, int modo, int movimentos);
void carregarHistorico(Historico **lista);
void exibirHistorico(Historico *lista);
void liberarHistorico(Historico *lista);

#endif