#ifndef PRODUTO 
#define PRODUTO

// ==== Estruturas ====

typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

typedef struct {
    int quant; // Quantidade atual de produtos
    int max; // Quantidade máxima a ser armazenada
    Produto* produtos; // Ponteiro para structs de produto
} ListaProduto;

void inserirProduto();

void listarProduto();

void editarProduto();

void removerProduto();

void salvarProdutoCSV();

void carregarProdutoCSV();

#endif