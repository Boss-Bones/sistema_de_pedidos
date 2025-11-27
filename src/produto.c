#include "produto.h"
#include "pedido.h"
#include <stdlib.h>
#include <stdbool.h>

// ==== Funções ====

Produto* consultarProduto(ListaProduto lprodutos, int id) {
    /*
    Implementação do caso de uso "Consultar produto"

    Nesse caso de uso, é extraído que o sistema deve retornar o produto com id correspondente, se existir...
    ...então, para isso, essa função foi criada para realizar exatamente isso

    Retorna:
    - Valor do tipo produto (Se tiver um produto com id correspondente)
    - NULL (Se não tiver um produto com id correspondente)
    */

    int i_produto = verificarProduto(lprodutos, id);

    if(i_produto != -1) {
        return lprodutos.produtos+i_produto;
    }
    return NULL;
}

int verificarProduto(ListaProduto lprodutos, int id) {
    /*
    Implementação do caso de uso "Analisar produto"

    Nesse caso de uso, é extraído que o sistema deve verificar se existe um produto com o id correspondente informado...
    ...então, para isso, essa função foi criada para retornar a existência do produto (segundo o id informado)

    Retorna:
    - indice do produto (Se tiver um produto com id correspondente)
    - -1 (Se não tiver um produto com id correspondente)
    */

    // Passa por todo o vetor
    for(int i = 0; i < lprodutos.quant; i++) {
        // Se o produto do indice i tiver o id correspondente
        if( (*(lprodutos.produtos+i)).id == id ) {
            // Retorna o indice
            return i;
        }
    }

    // Passou por todo o vetor e não encontrou nenhum com id correspondente, retorna -1
    return -1;
}

bool analisarProduto(ListaProduto *pdt, int id)
{
    int i;
    // verificar se o id do produto já existe usando pdt->produtos[i]

    for(i = 0; i < pdt->quant; i++)
    {
        if(pdt->produtos[i].id == id)
        {
            return true; // produto existe
        }
    }

    return false;
}

// Clebio, te orientei pelo Whatsapp, me fale qualquer dúvida
bool inserirProduto(Produto prd, ListaProduto *pdt){

    if(verificarProduto(prd.id) == -1){

        if(pdt->max == pdt->quant){
            pdt->produtos = (Produto*)realloc(pdt->produtos, (pdt->max + 10)*sizeof(Produto));
            pdt->max = pdt->max + 10;
        }

        *((pdt->produtos)+(pdt->quant)) = prd;
        pdt->quant++;

        return true;
    }

    return false;
}

// Clebio, essa função pode ser reescrita em ncurses na interface.c, estarei pensando antes de remover

void editarProduto(){}

// Clebio, no atual momento ainda não é possĩvel criar essa função
bool removerProduto(ListaProduto *pdt, int idremove){
    int indice, j=0, existe=0, existepedido=0;

    indice = verificarProduto(idremove);
    if(indice == -1){
        return false;
    }

    // verificarPedido();
    if(verificarPedido() != -1){
        return false;
    }

    for(int i=indice; i<(pdt->quant - 1); i++){
        *((pdt->produtos)+i) = *((pdt->produtos)+i+1);
    }
    pdt->quant--;

    return true;

}

void salvarProdutoCSV(){}

void carregarProdutoCSV(){}



//limp() é uma funçao pra limpar o buffer, tirar os "\n" que atrapalham as proximas leituras, a funcao deveria ficar em outro lugar, isso e um rascunho
//limp() tambem serve pra tirar o "\n" do final da string, caso ela seja menor que o tamanho máximo
void limparCBarraN(char *s)
{
    // se a string for menor que o tamanho definido, o "\n" ficou na string
    s[strcspn(s,"\n")] = '\0';

    // se a string for maior que o tamanho maximo, o "\n" esta no buffer
    if(strlen(s) == 99)
    {
        int l;
        while((l = getchar()) != '\n' && l != EOF );
    }
}
