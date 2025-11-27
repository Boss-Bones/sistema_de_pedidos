#include "pedido.h"
#include "cliente.h"
#include "produto.h"
#include <stdbool.h>
#include <stdlib.h>


// ==== Funções ====



int verificarPedido(int id, ListaPedido *lpedidos) {
    
    //Implementação do caso de uso "Analisar pedido"

    
    for(int i = 0; i < lpedidos->quant; i++) {
        // Se o produto do indice i tiver o id correspondente
        if( lpedidos->pedidos[i].id == id ) {
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

// função para colocar um pedido na lista de pedidos

bool cadastrarPedido(pedido novoPdd, ListaPedido *pdd){

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
    */

    // logica do Lucas

    if(pdd == NULL)
    {
        return false; // segurança
    }

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

    pdd->pedidos[pdd->quant] = novoPdd;

    pdd->quant++;
    
    return true;
}

// função para cadastrar itempedido

bool cadastrarItemPedido(pedido *pdd, ItemPedido novo)
{
    if(pdd == NULL) // segurança
    {
        return false;
    }

    // verificando se tem espaço no vetor de itens desse pedido

    if(pdd->quant_itens == pdd->max_itens)
    {   // mesma logica de inserção usada anteirormente em cliente
        int nova_capacidade = (pdd->max_itens == 0) ? 10 : pdd->max_itens * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(ItemPedido);

        ItemPedido *temp = realloc(pdd->itens, novo_tamanho_bytes); // ponteiro temporário para segurança

        // vendo se o realloc deu certo

        if(temp == NULL)
        {
            return false;
        }

        //se deu certo pode usar a nova capacidade

        pdd->max_itens = nova_capacidade;
        pdd->itens = temp;

    }

    // inserindo o item

    pdd->itens[pdd->quant_itens] = novo;
    pdd->total += novo.subtotal; // atualizando o preço do pedido
    pdd->quant_itens++; // atualizar a quantidade de itens

    return true;

}

// função para remover pedido

bool removerPedido(ListaPedido *pdd, int idRemove)
{
    // Parecido com remover cliente
    int indice;

    indice = verificarPedido(idRemove, pdd); // descobrindo onde está o pedido que quer remover

    if(indice == -1){ // id não exsite
        return false;
    }

    // removendo todos os itens de pedido desse pedido

    free(pdd->pedidos[indice].itens); // libera o espaço alocado dinamicamente

    // removendo o pedido

    int i;
    for(i = indice; i<pdd->quant-1; i++)
    {
        pdd->pedidos[i] = pdd->pedidos[i+1];
    }
    pdd->quant--;

    return true;

}