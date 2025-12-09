#ifndef PEDIDO 
#define PEDIDO
#include <stdbool.h>

typedef struct ItemPedido
{
    /* Esta é uma estrutura (struct) associativa. Ela representa um item específico dentro de um pedido, conectando
    as estruturas Pedido e Produto. */

    
     
     
    int pedidoId; // mesmo id do pedido
    int produtoId; // O id do produto que está sendo comprado.
    int quantidade; // A quantidade daquele produto específico neste pedido.
    double subtotal; // O valor total para este item (quantidade multiplicada pelo preco do Produto).

} ItemPedido;

typedef struct pedido
{
    // Representa uma ordem de compra feita por um cliente.

    int id; // Um número de identificação único para cada pedido.
    int clienteId; /* Armazena o id do cliente que fez o pedido, servindo como uma chave estrangeira
    para a classe Cliente. */
    char data[11]; // A data em que o pedido foi realizado (ex: "dd/mm/aaaa").
    double total; // O valor total do pedido, que seria a soma dos subtotais de todos os itens.
    ItemPedido *itens;
    int quant_itens; // Quantos itens esse pedido tem agora
    int max_itens;   // Qual o tamanho atual do realloc 

} pedido;

typedef struct {
    int quant;
    int max;
    pedido *pedidos;
} ListaPedido;


// Coloque a prototipação da função aqui

int verificarPedido(int id, const ListaPedido *lpedidos);

bool analisarPedido(const ListaPedido *pdd, int id);

bool cadastrarItemPedido(pedido *pdd, ItemPedido novo);

bool removerPedido(ListaPedido *pdd, int idRemove);

pedido *consultarPedido (ListaPedido *pdd, int id);

bool removerItemPedido(pedido *pdd, int id_produto);

bool cadastrarPedido(pedido novoPdd, ListaPedido *pdd);

#endif