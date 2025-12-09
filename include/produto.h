#ifndef PRODUTO 
#define PRODUTO
#include <stdbool.h>
#include "pedido.h"
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

bool inserirProduto(Produto prd, ListaProduto *pdt);

bool removerProduto(ListaProduto *pdt, ListaPedido *pdd, int idremove);

int verificarProduto(const ListaProduto *lprodutos, int id);

bool analisarProduto(const ListaProduto *pdt, int id);

Produto *consultarProduto(ListaProduto *lprodutos, int id);

#endif