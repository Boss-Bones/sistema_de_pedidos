#include "persistencia.h"
#include "cliente.h"
#include "pedido.h"
#include "produto.h"
#include <stdlib.h>

/*
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
----------------------------- Funções de clientes ------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/


int salvarCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj){
    FILE *pt;

    pt = fopen("clientes.csv", "w");

    // uma boa prática que eu vi no livro 
    if(pt == NULL){
        return -1; // se achar que deve ser outro valor pode mudar
    }
    
    for(int i=0; i<clt->quant; i++){
        fprintf(pt, "%d;%s;%s;%s;%d", clt->clientes[i].id, clt->clientes[i].nome, clt->clientes[i].endereco, clt->clientes[i].telefone, clt->clientes[i].tipo);

        if(clt->clientes[i].tipo == 0)
        {
            // fazwr funçoes buscar_cpf() e buscar_cnpj()
            fprintf(pt,";%s\n", cpf->cpfs[buscar_cpf(clt->clientes[i].id, cpf)].cpf);
        } else {
            fprintf(pt,";%s\n", cnpj->cnpjs[buscar_cnpj(clt->clientes[i].id, cnpj)].cnpj);
        }

        /* cada colchete acima chama as funções de buscar cpf e cnpj, que recebem como argumentos
          =o id do cliente atual, e o ponteiro pro vetor de cpf/cnpj, então percorre a lista até encontrar a posição do mesmo id na lista cpf/cnpj retornando a posição*/

        // obs csv usa ponto como separador decimal, Excel do Brasil usa vírgula, fica essa dúvida em aberto
    }

    fclose(pt);

    return 0;
    
} // fim de salavrCliente



int carregarCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj){
    FILE *pt;

    int contador=0;
    char tempchar[400];

    // inicialização de segurança
    clt->quant=0;
    clt->max=0;
    clt->clientes = NULL;

    pt = fopen("clientes.csv", "r");

    // se o arquivo não existe
    if(pt == NULL){
        return -1;
    }

    // se o arquivo existe

    /* tempchar é uma variável auxiliar que irá pegar cada linha do arquivo para contar quantos registros no total existem no arquivo, e cada linha escrita em tempchar incrementa a variável
    contador, que serve para armazenar o valor total de registros */
    while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
        contador++;
    }

    // se o arquivo existe mas está vazio, aloca 10 espaços na memória
    if(contador == 0){
        clt->clientes = (cliente*)malloc( 10 * sizeof(cliente));
        clt->max = 10;
        fclose(pt);
        return 0;
    }

    /* Uma vez que contador tem o valor total de registros e é maior que 0,
    ela serve pra auxiliar a alocação de memória para o vetor de produtos e
    guardar a quantidade atual na variável de quantidade em ListaProduto, pdt->quant */

    clt->quant = contador;
    clt->max = contador*2;
    clt->produtos = (cliente*)malloc(clt->max * sizeof(cliente));

    fseek(pt, 0, SEEK_SET);
    /* essa função acima serve pra retornar ao início do arquivo, pois depois da checagem da
    quantidde de registros, o ponteiro de leitura estará posicionado no final do arquivo */

    
    /* Por fim o for usa a variável contador como base para saber quantos registros serão
    inseridos no vetor */

    for(int i=0; i<(clt->quant); i++){
        fscanf(pt, "%d;%99[^;];%199[^;];%19[^;];%d\n", &clt->clientes[i].id, clt->clientes[i].nome, clt->clientes[i].endereco, clt->clientes[i].telefone, clt->clientes[i].tipo);
        /* 99[^;] = leia tudo enquanto não for ponto e vírgula,
        fica no lugar de %s para ler a descrição toda enão parar de ler nos espaços */


        // falta a parte dos cpf e cnpj
    }

    fclose(pt);

    return 0;
} // fim de carregarCliente



/*
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
----------------------------- Funções de produtos ------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/


int salvarProduto(ListaProduto *pdt){
    FILE *pt;

    pt = fopen("produtos.csv", "w");

    // uma boa prática que eu vi no livro 
    if(pt == NULL){
        return -1; // se achar que deve ser outro valor pode mudar
    }
    /* essa função deve pegar o vetor de struct do tipo Produto que pertence a struct ListaProduto
    e usar a variável quant de ListaProduto pra saber quantos registros ela deve salvar no arquivo*/
    for(int i=0; i<pdt->quant; i++){
        fprintf(pt, "%d;%s;%lf;%d\n", pdt->produtos[i].id, pdt->produtos[i].descricao, pdt->produtos[i].preco, pdt->produtos[i].estoque);

        // obs csv usa ponto como separador decimal, Excel do Brasil usa vírgula, fica essa dúvida em aberto
    }

    fclose(pt);

    return 0;
    
}


int carregarProduto(ListaProduto *pdt){
    FILE *pt;

    int contador=0;
    char tempchar[200];

    // inicialização de segurança
    pdt->quant=0;
    pdt->max=0;
    pdt->produtos = NULL;

    pt = fopen("produtos.csv", "r");

    // se o arquivo não existe
    if(pt == NULL){
        return -1;
    }

    // se o arquivo existe

    /* tempchar é uma variável auxiliar que irá pegar cada linha do arquivo para contar quantos registros no total existem no arquivo, e cada linha escrita em tempchar incrementa a variável
    contador, que serve para armazenar o valor total de registros */
    while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
        contador++;
    }

    // se o arquivo existe mas está vazio, aloca 10 espaços na memória
    if(contador == 0){
        pdt->produtos = (Produto*)malloc( 10 * sizeof(Produto));
        pdt->max = 10;
        fclose(pt);
        return 0;
    }

    /* Uma vez que contador tem o valor total de registros e é maior que 0,
    ela serve pra auxiliar a alocação de memória para o vetor de produtos e
    guardar a quantidade atual na variável de quantidade em ListaProduto, pdt->quant */

    pdt->quant = contador;
    pdt->max = contador*2;
    pdt->produtos = (Produto*)malloc( pdt->max * sizeof(Produto));

    fseek(pt, 0, SEEK_SET);
    /* essa função acima serve pra retornar ao início do arquivo, pois depois da checagem da
    quantidde de registros, o ponteiro de leitura estará posicionado no final do arquivo */

    
    /* Por fim o for usa a variável contador como base para saber quantos registros serão
    inseridos no vetor */

    for(int i=0; i<(pdt->quant); i++){
        fscanf(pt, "%d;%99[^;];%lf;%d\n", &pdt->produtos[i].id, pdt->produtos[i].descricao, &pdt->produtos[i].preco, &pdt->produtos[i].estoque);
        /* 99[^;] = leia tudo enquanto não for ponto e vírgula, fica no lugar de %s para ler a descrição toda e
        não parar de ler nos espaços */
    }

    fclose(pt);

    return 0;
}



/*
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
----------------------------- Funções de pedidos -------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/

int salvarPedido(ListaProduto *pdt){
    //em andamento...
    
}

int carregarPedido(ListaProduto *pdt){
    //em andamento...
    
}

