#include "pedido.h"

typedef struct pedido
{
    // Representa uma ordem de compra feita por um cliente.

    int id; // Um número de identificação único para cada pedido.
    int clienteId; /* Armazena o id do cliente que fez o pedido, servindo como uma chave estrangeira
    para a classe Cliente. */
    char data[11]; // A data em que o pedido foi realizado (ex: "dd/mm/aaaa").
    double total; // O valor total do pedido, que seria a soma dos subtotais de todos os itens.

} pedido;

typedef struct ItemPedido
{
    /* Esta é uma estrutura (struct) associativa. Ela representa um item específico dentro de um pedido, conectando
    as estruturas Pedido e Produto. */

     int pedidoId; // O id do pedido ao qual este item pertence.
     int produtoId; // O id do produto que está sendo comprado.
     int quantidade; // A quantidade daquele produto específico neste pedido.
     double subtotal; // O valor total para este item (quantidade multiplicada pelo preco do Produto).

} ItemPedido;



// ==== Funções ====



int verificarPedido(int id) {
    
    //Implementação do caso de uso "Analisar pedido"

    
    for(int i = 0; i < quant_pedidos; i++) {
        // Se o produto do indice i tiver o id correspondente
        if( (*(pedidos+i)).id == id ) {
            // Retorna o indice
            return i;
        }
    }

    // Passou por todo o vetor e não encontrou nenhum com id correspondente, retorna -1
    return -1;
}