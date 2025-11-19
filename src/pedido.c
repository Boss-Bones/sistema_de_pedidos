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
    ItemPedido *itens;

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


bool analisarPedido(ListaPedido *pdd, int id)
{
    int i;
    // verificar se o id do pedido já existe usando pdd->pedidos[i]

    for(i = 0; i < pdd->quant; i++)
    {
        if(pdd->pedidos[i].id == id)
        {
            return false; // id já cadastrado
        }
    }

    return true;
}

/* em andamento*/

bool cadastrarPedido(ListaPedido *pdd){

    /*
    1) usuário informa o código identificador do pedido (interface)
        - função da interface chama função analisarPedido()
            se o código ja existir, retornar mensagem de erro e retornar para tela de cadastro
            para o usuário preencher o campo de código novamente
        - se não existir, próximo passo

    2) usuário informa o código do cliente do referido pedido (interface)
        - função da interface chama função buscar_id_cliente()
            se o código não existir, retornar mensagem de erro e retornar para tela de cadastro
            para o usuário preencher o campo de código de cliente novamente
        - se existir, próximo passo

    3) usuário informa o código do produto a ser cadastrado como item do pedido (interface)
        - continua... precisa da função analisarProduto
    */
    
    // verifica se o código do pedido já existe. Se existir, deve retornar uma mensagem de erro
    
    
    return false;
}

