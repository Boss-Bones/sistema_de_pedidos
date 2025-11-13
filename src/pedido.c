#include "pedido.h"
#include "cliente.h"
#include "produto.h"
#include <stdbool.h>

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

typedef struct {
    int quant;
    int max;
    pedido *pedidos;
} ListaPedido;



// ==== Funções ====



int verificarPedido(int id, ListaPedido lpedidos) {
    
    //Implementação do caso de uso "Analisar pedido"

    
    for(int i = 0; i < lpedidos.quant; i++) {
        // Se o produto do indice i tiver o id correspondente
        if( (*(lpedidos.pedidos+i)).id == id ) {
            // Retorna o indice
            return i;
        }
    }

    // Passou por todo o vetor e não encontrou nenhum com id correspondente, retorna -1
    return -1;
}

/* desculpe, eu deixei 3 funções porque, primieiro se o código do pedido existe.Se existe 
deve retornar uma mensagem de erro. Mas se não existe, então o usuário insere o código do cliente,
aí nessa parte eu imaginei que seria outra vez papel da interface. Então se o cliente não existir,
mensagem de erro, mas se existir, o usuário entra outra vez com outro código do produto, que eu
imaginei que outra vez seria papel da interface. Na última, eu me confundi e não consegu implementar*/

bool cadastrarPedido1(int id, ListaPedido pdd){
    
    // verifica se o código do pedido já existe. Se existir, deve retornar uma mensagem de erro
    if(verificarPedido(id, pdd.pedidos) == -1){
        
        return true;
    }
    
    return false;
}

bool cadastrarPedido2(int id, ListaCliente clt){
    
        
    // verifica se o código do cliente existe. Se não existir, deve retornar uma mensagem de erro
    if(buscar_id_cliente(clt, idclt) != -1){
        return true;
    }
    
    return false;
}

bool cadastrarPedido3(int id, ListaProduto prd){
    
        
    /* desculpa essa ficou incompleta, eu fiquei confuso se vai ter uma lista de Itens pedidos
    ou usa a lista de pedidos */
}