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

    
     //int pedidoId; // O id do pedido ao qual este item pertence.
     /* duvida, ItemPedido faz parte de pedido, então porque repetir o id do pedido aqui? */

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

bool cadastrarPedido(int idpedido, int idcliente, int idproduto, int quant, double preco, ListaPedido *pdd){

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
        - função da interface chama função analisarProduto()
            se o código não existir, retornar mensagem de erro e retornar para tela de cadastro
            para o usuário preencher o campo de código de cliente novamente
        - se existir, próximo passo

    4) o usuário pode inserir mais de um produto na função da interface, pois um pedido pode
    ter vários produtos, a função da interface deveria pegar o valor unitário do produto
    então esta função aqui pega esse valor em double preco para calcular o subtotal
    */

    // logica do Lucas
     if (pdd->quant == pdd->max)
    {
        // dobrar o tamanho do vetor (otimização)
        // se tiver 0 adotar o tamanho padrão 10

        int nova_capacidade = (pdd->max == 0) ? 10 : pdd->max * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(pedido);

        // realloc para aumentar o tamanho do vetor de clientes
        // usei um ponteiro temporario para armazenar a nova lista por motivos de segurança

        pedido* temp = realloc(pdd->pedidos, novo_tamanho_bytes);
        
        // ver se o realloc deu certo

        if(temp == NULL)
        {
            return false;
        }
        // se chegar aqui é porque deu certo pode usar o novo tamanho do vetor de clientes

        pdd->pedidos = temp;
        pdd->max = nova_capacidade;

    }

    pdd->pedidos[pdd->quant].total = idpedido;

    pdd->pedidos[pdd->quant].id = idpedido;
    pdd->pedidos[pdd->quant].clienteId = idcliente;
    // não sei de onde a data vai vir
    pdd->pedidos[pdd->quant].itens->produtoId = idproduto;
    pdd->pedidos[pdd->quant].itens->quantidade = quant;
    pdd->pedidos[pdd->quant].itens->subtotal = preco*quant;
    //pdd->pedidos[pdd->quant].total = não sei como colocar o total aqui

    pdd->quant++;
    
    return true;
}

