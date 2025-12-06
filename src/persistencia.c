#include "persistencia.h"
#include "cliente.h"
#include "pedido.h"
#include "produto.h"
#include <stdlib.h>
#include <string.h>

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

    if(pt == NULL){
        return -1;
    }
    
    for(int i=0; i<clt->quant; i++){
        fprintf(pt, "%d;%s;%s;%s;%d;", clt->clientes[i].id, clt->clientes[i].nome, clt->clientes[i].endereco, clt->clientes[i].telefone, clt->clientes[i].tipo);

        /* Fiz uns testes locais em outro programa, e funcionou eu colocando
        o nome da constante na comparaçao, e não o numero inteiro dela*/
        if(clt->clientes[i].tipo == TIPO_PESSOA_FISICA)
        {
            // fazwr funçoes buscar_cpf() e buscar_cnpj()
            fprintf(pt,"%s\n", cpf->cpfs[buscar_cpf(clt->clientes[i].id, cpf)].cpf);
        } else {
            fprintf(pt,"%s\n", cnpj->cnpjs[buscar_cnpj(clt->clientes[i].id, cnpj)].cnpj);
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

    int contador=0, temptipo;
    char tempchar[400], tempregistro[19];

    pt = fopen("clientes.csv", "r");

    // se o arquivo não existe
    if(pt == NULL){
        return -1;
    }

    // inicialização de segurança
    clt->quant=0;
    clt->max=0;
    clt->clientes = NULL;

    cpf->quant = 0;
    cpf->max = 0;
    cpf->cpfs = NULL;

    cnpj->quant = 0;
    cnpj->max = 0;
    cnpj->cnpjs = NULL;

    /*se ainda assim não funcionar, tentar colocar mais parametros e alterar desse modo:
    cl = NULL;
    clt->clientes = cl;
    cp = NULL;
    clp->cpfs = cp;
    cnp = NULL;
    cnpj->cnpjs = cl;

    aqui cl, cp, e cnp, seriam ponteiros dos vetores de cliente, cpf e cnpj, respectivamente. Atenção, eles não seriam ponteiros pras variaveis tipo
    ponteiro que estão nas LISTAS, mas sim ponteiros dos vetores
    */


    // se o arquivo existe

    /* tempchar é uma variável auxiliar que irá pegar cada linha do arquivo para contar quantos registros no total existem no arquivo, e cada linha escrita em tempchar incrementa a variável contador, que serve para armazenar o valor total de registros. Talvez eu podia usar clt->quant direto nesse while, mas deixa. */
    while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
        contador++;
    }

    // se o arquivo existe mas está vazio, aloca 10 espaços na memória
    if(contador == 0){
        clt->clientes = (cliente*)malloc( 10 * sizeof(cliente));
        clt->max = 10;
        cpf->cpfs = (PessoaFisica*)malloc( 10 * sizeof(PessoaFisica));
        cpf->max = 10;
        cnpj->cnpjs = (PessoaJuridica*)malloc( 10 * sizeof(PessoaJuridica));
        cnpj->max = 10;
        fclose(pt);
        return 0;
    }

    /* Uma vez que contador tem o valor total de registros e é maior que 0,
    ela serve pra auxiliar a alocação de memória para os vetores de clientes, cpf e cnpj e guardar a quantidade atual na variável de quantidade clt->quant */

    clt->quant = contador;
    clt->max = contador*2;
    clt->clientes = (cliente*)malloc(clt->max * sizeof(cliente));

    //cpf->quant = contador;
    cpf->max = contador*2;
    cpf->cpfs = (PessoaFisica*)malloc(cpf->max * sizeof(PessoaFisica));

    //cnpj->quant = contador;
    cnpj->max = contador*2;
    cnpj->cnpjs = (PessoaJuridica*)malloc(cpf->max * sizeof(PessoaJuridica));

    /*Obs. cpf e cnpj quant não devem receber o mesmo valor de contador, pois por exmeplo, se houver 10 clientes, 7 fisica, 3 juridica, não dá pra dizer que há
    10 cpfs e 10 cnpjs. Deixei como comentário acima esse erro*/


    fseek(pt, 0, SEEK_SET);
    /* essa função acima serve pra retornar ao início do arquivo, pois depois da checagem da
    quantidde de registros, o ponteiro de leitura estará posicionado no final do arquivo */

    
    /* Por fim o for usa a variável clt->quant que recebeu contador como base
    para saber quantos registros serão inseridos no vetor. Como não é possível,
    por exemplo, haver mais cpfs do que clientes, as variaveis
    cpf->quant e cnpj->quant continuam como 0 e só são incrementadas quando
    houver um cliente do seu tipo*/

    for(int i=0; i < clt->quant; i++){
        fscanf(pt, "%d;%99[^;];%199[^;];%19[^;];%d;", &clt->clientes[i].id, clt->clientes[i].nome, clt->clientes[i].endereco, clt->clientes[i].telefone, temptipo);
        /* 99[^;] = leia tudo enquanto não for ponto e vírgula,
        fica no lugar de %s para ler a descrição toda enão parar de ler nos espaços */

        /* Aqui ele pega o valor inteiro que vem do arquivo e converte para
        o tipo da Enum, isso serve apra evitar comportamento indefinido*/
        clt->clientes[i].tipo = (TipoCliente)temptipo;
        


    /* a função vai verificar se o cliente é uma pessoa física ou uma pessoa jurídica para saber em qual vetor salvar o cpf ou cnpj. Aqui no final
    a condição é enquanto não for barra n "\n" pois como é o último dado da linha
    não haverá mais ";" nessa linha */
        if(clt->clientes[i].tipo == TIPO_PESSOA_FISICA){
            fscanf(pt, "%14[^\n]\n", cpf->cpfs[cpf->quant].cpf);
            cpf->quant++;
        } else {
            fscanf(pt, "%18[^\n]\n", cnpj->cnpjs[cnpj->quant].cnpj);
            cnpj->quant++;
        }
    } // fim do for

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

    if(pt == NULL){
        return -1;
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

    // se o arquivo não existe
    if(pt == NULL){
        return -1;
    }

    // se o arquivo existe

    // inicialização de segurança
    pdt->quant=0;
    pdt->max=0;
    pdt->produtos = NULL;

    pt = fopen("produtos.csv", "r");

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

int salvarPedido(ListaPedido *pdd){
    FILE *pt;

    pt = fopen("pedidos.csv", "w");

    if(pt == NULL){
        return -1;
    }

    /* essa função deve pegar o vetor de struct do tipo Pedido que pertence a struct ListaPedido
    e usar a variável quant de ListaPedido pra saber quantos registros ela deve salvar no arquivo*/
    for(int i=0; i<pdd->quant; i++){
        fprintf(pt, "%d;%d;%s;%lf\n", pdd->pedidos[i].id, pdd->pedidos[i].clienteId, pdd->pedidos[i].data, pdd->pedidos[i].total);

        // obs csv usa ponto como separador decimal, Excel do Brasil usa vírgula, fica essa dúvida em aberto
    }

    fclose(pt);

    return 0;
    
}

int salvarItemPedido(ListaPedido *pdd){
    FILE *pt;

    pt = fopen("itenspedidos.csv", "w");

    if(pt == NULL){
        return -1;
    }

    /* essa função deve pegar o vetor de struct do tipo Pedido que pertence a struct ListaPedido
    e usar a variável quant de ListaPedido pra saber quantos registros ela deve salvar no arquivo*/

    for(int i=0; i<pdd->pedidos[i].quant; i++){
        for (int j = 0; j < pdd->pedidos[i].quant_itens; j++)
        {
            fprintf(pt,"%d;%d;%d;%lf", pdd->pedidos[i].itens[j].pedidoId, pdd->pedidos[i].itens[j].produtoId, pdd->pedidos[i].itens[j].quantidade, pdd->pedidos[i].itens[j].subtotal);
        }
    }

    fclose(pt);

    return 0;
    
}

int carregarPedido(ListaPedido *pdd){
    FILE *pt;

    int contador=0;
    char tempchar[200];

    pt = fopen("pedidos.csv", "r");

    if(pt == NULL){
        return -1;
    }

    pdd->quant=0;
    pdd->max=0;
    pdd->pedidos=NULL;

    while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
        contador++;
    }

    if(contador == 0){
        pdd->pedidos = (pedido*)malloc(10 * sizeof(pedido));
        pdd->max = 10;
        fclose(pt);
        return 0;
    }

    pdd->quant = contador;
    pdd->max = contador*2;
    pdd->pedidos = (pedido*)malloc( pdt->max * sizeof(pedido));

    fseek(pt, 0, SEEK_SET);

    for(int i=0; i<(pdd->quant); i++){
        fscanf(pt, "%d;%d;%s;%lf\n", &pdd->pedidos[i].id, &pdd->pedidos[i].clienteId, pdd->pedidos[i].data, &pdd->pedidos[i].total);
    }

    fclose(pt);

    return 0;
    
}

int carregarItemPedido(ListaPedido *pdd){
    
    FILE *pt;

    int contador=0, idpedidotemp, idprodutotemp, quantidadetemp;
    double  subtotaltemp;
    char tempchar[200];

    pt = fopen("itenspedidos.csv", "r");

    if(pt == NULL){
        return -1;
    }

    // inicialização de segurança de quant, max e o ponteiros de itens de cada posição do vetor de pedidos
    for (int i = 0; i < pdd->quant; i++)
    {
        pdd->pedidos[i].itens = NULL;
        pdd->pedidos[i].quant_itens = 0;
        pdd->pedidos[i].max_itens = 0; 
    }

    while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
        contador++;
    }

    /* se tiver 0 linhas, cada vetor de pedido tem um vetor de itens pedidos,
    este for aloca 10 espaços no vetor de itens pedidos que está contido em cada posição do
    vetor de pedidos, e inicia o max_itens de cada posição com 10 */
    if(contador == 0){
        for (int i = 0; i < pdd->quant; i++)
        {
            pdd->pedidos[i].itens = (ItemPedido*)malloc(10*sizeof(ItemPedido));
            pdd->pedidos[i].max_itens = 10;
            fclose(pt);
            return 0;
        }
    }

    

    /* se tiver mais que 0 linhas, cada vetor de pedido tem um vetor de itens pedidos,
    este for atribui à max_itens o valor (quantidade de linhas * 2) e aloca essa quantidade de
    espaços no vetor de itens pedidos que está contido em cada posição do vetor de pedidos,
    e inicia o max_itens de cada posição com 10 */
    for (int i = 0; i < pdd->quant; i++)
    {
        /* a lógica pra saber a quantidade de itens é muito difícil, pois cada pedido pode ter
        uma quantidade variada e diferente de itens pedidos, por isso a quantidade de
        itens pedidos é conhecida abaixo, mas aqui as quant_itens de cada posição
        já estão iniciada com 0 na inicialização de segurança*/
        pdd->pedidos[i].max_itens = contador * 2; 
        pdd->pedidos[i].itens = (ItemPedido*)malloc(pdd->pedidos[i].max_itens * sizeof(ItemPedido));
    }

    for(int i=0; i<pdd->quant; i++){

        fseek(pt, 0, SEEK_SET);

        pdd->pedidos[i].quant_itens = 0;

        while((fgets(tempchar, sizeof(tempchar), pt)) != NULL){
            fscanf(pt, "%d;%d;%d;%lf", &idpedidotemp, &idprodutotemp, &quantidadetemp, &subtotaltemp);
            if(pdd->pedidos[i].id == idpedidotemp){
                pdd->pedidos[i].itens[pdd->pedidos[i].quant_itens].pedidoId = idpedidotemp;
                pdd->pedidos[i].itens[pdd->pedidos[i].quant_itens].produtooId = idpedidotemp;
                pdd->pedidos[i].itens[pdd->pedidos[i].quant_itens].quantidade = idpedidotemp;
                pdd->pedidos[i].itens[pdd->pedidos[i].quant_itens].subtotal = idpedidotemp;

                pdd->pedidos[i].quant_itens++;

            } // fim if
        } // fim while
    } // fim for

    fclose(pt);

    return 0;

    /* O for acima deve percorrer cada posição do vetor de pedidos;
    Por exemplo, na posição o, pdd->pedidos[0], ele inicia a pdd->pedidos[0].quant_itens com 0,
    então usa o while pra checar linha por linha. Cada laço do while ele guarda a linha atual em
    4 variáveis temporarias,
    (pois são 4 valores pra itens pedidos, "pedidoId, ProdutoId, quantidade, subtotal")
    em seguida ele compara o id da posição atual do vetor de pedidos (nesse exemplo posição 0)
    com o id que veio do arquivo, e ele só coloca os valores no subvetor de itens pedidos que está
    na posição atual do vetor de pedidos se os ids forem iguais, e somente se isso acontecer
    é que ele incrementa a variável quant_itens da atual posição do vetor de pedidos.
    pdd->pedidos[0].quant_itens

    É por isso que o próprio valor de pdd->pedidos[0].quant_itens é usado dentro do colchete, por exemplo:
    pdd->pedidos[i].itens[   pdd->pedidos[i].quant_itens    ].pedidoId = idpedidotemp;
    porque no começo ela é 0, o primeiro item pedido que entrar no subvetor irá na posição 0
    pdd->pedidos[i].itens[   0    ].pedidoId = idpedidotemp;


    então pdd->pedidos[i].quant_itens é incrementado e fica com o valor 1

    Na próxima vez que o while guardar valores no subvetor, será na posição:
    pdd->pedidos[i].itens[   1    ].pedidoId = idpedidotemp;
    pois nesse momento,     pdd->pedidos[i].quant_itens = 1

    Então nele continua o processo e percorre as linhas seguintes do arquivo,
    e só coloca os valores no subvetor se os ids forem iguais.

    Quando ele percorrer todas as linhas, apenas os itens pedidos com o mesmo id da posição atual
    do vetor pedidos são colocados no subvetor, e também fica guardado a quantidade de itens em
    pdd->pedidos[0].quant_itens, que só foi incrementada quando o while guardava valores no subvetor

    Então o while chega no fim do arquivo e encerra. O for incrementa a int i e vai pra próxima
    posição do vetor de pedidos. na posição pdd->pedidos[1] ele inicia a pdd->pedidos[1].quant_itens
    com 0, recomeça o while e repete o mesmo processo, apenas incrementando pdd->pedidos[1].quant_itens se o id da posição
    pdd->pedidos[1].id for igual ao id da linha atual do while
    */
    
}
