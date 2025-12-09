#include "produto.h"
#include "pedido.h"
#include <stdlib.h>
#include <stdbool.h>

// ==== Funções ====

Produto *consultarProduto(ListaProduto *lprodutos, int id)
{
    /*
    Implementação do caso de uso "Consultar produto"

    Nesse caso de uso, é extraído que o sistema deve retornar o produto com id correspondente, se existir...
    ...então, para isso, essa função foi criada para realizar exatamente isso

    Retorna:
    - Valor do tipo produto (Se tiver um produto com id correspondente)
    - NULL (Se não tiver um produto com id correspondente)
    */

    int i_produto = verificarProduto(lprodutos, id);

    if (i_produto != -1)
    {
        return lprodutos->produtos + i_produto;
    }
    return NULL;
}

int verificarProduto(const ListaProduto *lprodutos, int id)
{
    /*
    Implementação do caso de uso "Analisar produto"

    Nesse caso de uso, é extraído que o sistema deve verificar se existe um produto com o id correspondente informado...
    ...então, para isso, essa função foi criada para retornar a existência do produto (segundo o id informado)

    Retorna:
    - indice do produto (Se tiver um produto com id correspondente)
    - -1 (Se não tiver um produto com id correspondente)
    */

    // Passa por todo o vetor
    for (int i = 0; i < lprodutos->quant; i++)
    {
        // Se o produto do indice i tiver o id correspondente
        if ((*(lprodutos->produtos + i)).id == id)
        {
            // Retorna o indice
            return i;
        }
    }

    // Passou por todo o vetor e não encontrou nenhum com id correspondente, retorna -1
    return -1;
}

bool analisarProduto(const ListaProduto *pdt, int id)
{
    int i;
    // verificar se o id do produto já existe usando pdt->produtos[i]

    for (i = 0; i < pdt->quant; i++)
    {
        if (pdt->produtos[i].id == id)
        {
            return true; // produto existe
        }
    }

    return false;
}

// Função de inserir produto

bool inserirProduto(Produto prd, ListaProduto *pdt)
{
    if (pdt == NULL) // segurança
    {
        return false;
    }

    if (pdt->quant == pdt->max) // verificar se tem espaço no vetor de produtos
    {
        int nova_capacidade = (pdt->max == 0) ? 10 : pdt->max * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(Produto);

        // ponteiro temporário para não perder a lista em caso de erro

        Produto *temp = realloc(pdt->produtos, novo_tamanho_bytes);

        // ver se o realloc deu certo

        if (temp == NULL)
        {
            return false;
        }

        pdt->produtos = temp;
        pdt->max = nova_capacidade;
    }

    pdt->produtos[pdt->quant] = prd;
    pdt->quant++;

    return true;
}

bool removerProduto(ListaProduto *pdt, ListaPedido *pdd, int idremove)
{
    if(pdt == NULL)
    {
        return false;
    }
    // esses dois ifs são para segurança
    if(pdd == NULL)
    {
        return false;
    }

    int indice = verificarProduto(pdt, idremove);
    if(indice == -1)
    {
        return false; // produto não encontrado
    }

    // ver se acha esse produto no vetor itens de cada pedido da lista de pedidos

    int i, j;
    for(i = 0; i<pdd->quant; i++)
    {
        for(j = 0; j<pdd->pedidos[i].quant_itens; j++)
        {
            if(idremove == pdd->pedidos[i].itens[j].produtoId)
            {
                return false; // achou um produto em um pedido
            }
        }
    }

    for(i = indice; i<pdt->quant-1; i++)
    {
        pdt->produtos[i] = pdt->produtos[i+1];
    }
    pdt->quant--;

    return true;

}
