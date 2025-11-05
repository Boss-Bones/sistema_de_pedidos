#include "produto.h"
#include <stdlib.h>

// ==== Estruturas ====

typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

// ==== Váriaveis ====

/*
A váriavel produtos deve armazenar os produtos de forma dinâmica, use alocação dinâmica, como o malloc()

Começe alocando um espaço inicial (ex.: 10), ao chegar ao limite, aumente o vetor usando realloc(), não faça de 1 em 1 (ex.: +10 depois de atingir o limite)
Como saber se chegou ao limite? Use quant_produtos e max_produtos para saber, guarde neles também para não se perder.
*/
int quant_produtos; // Quantidade de produtos no vetor 'produtos'
int max_produtos; // Tamanho do vetor 'produtos'
Produto* produtos; // Vetor de produtos

// ==== Funções ====

void inserirProduto(Produto *pdt){
    
    // funcao de leitura, que pelo que o Joao Fernando me disse, chamará essa função
    (pdt+quant_produtos) // load
    
}

void listarProduto(Produto *pdt){

    for(int i=0; i<quant_produtos; i++){
        printf("%d - %s - %lf - %d\n", (pdt+i)->id, (pdt+i)->descricao, (pdt+i)->preco, (pdt+i)->estoque);
    }
}

void editarProduto(){}

void removerProduto(Produto *pdt, int idremove){
    int i=0, j=0, existe=0, existepedido=0;

    /* enquanto não achar um codigo igual ao codigo que o usuario entrou
    ou não tiver chegado no final do vetor, continua lendo. Aqui é E(&&)
    ao invés de OU(||) por causa da Lei de De Morgan*/

    /*tinha que ter mais um argumento que vem de pedidos, mas
    não sei exatamente de onde ele vem, ai não sei que parametro colocar ali*/


    // checa se existe um produto com o código que o usuário entrou
    while(i<quant_produtos){
        if(pdt->id == idremove)
        {
            existe++;
            break;
        }
    }

    /* trabalha com duas opções, se o produto existe, então ele verifica
    se existe um pedido com esse produto, que era pra ser o while ali dentro,
    se não existe, vai informar uma mensagem de erro que o produto não existe */
    if(existe>0)
    {
        while()
        /* deve haver algum parametro que eu não sei como colocar, se eu colocar um parametro pedido *ponteiro funciona? Pois como a struct
        pedido nao está aqui, o VS Code fica acusando erro */
        if(existepedido>0){
            printf("Produto não pode ser excluído pois existe um pedido!\n");
        } else {
            /* não sei como implementar o pedido de confirmação de exclusão aqui, 
            seria com uma simples opção de scanf ou algo relacionado ao ncurses?*/
        }
    } else {
        printf("Erro, id não cadastrado!\n");
    }
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