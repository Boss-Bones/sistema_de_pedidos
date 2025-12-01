#include "cliente.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


// Função para verificar se o id que o cliente digitou pode ser usado

bool verificar_id_cliente(ListaCliente *clt, int id)
{
    int i;
    // verificar se o id do cliente já existe usando clt->clientes[i]

    for(i = 0; i < clt->quant; i++)
    {
        if(clt->clientes[i].id == id)
        {
            return false; // id já cadastrado
        }
    }

    return true;
}

// Função que busca id de um cliente que já existe (ajuda no consultar e remover cliente)

int buscar_id_cliente(ListaCliente* clt, int id)
{
    int i;
    for(i = 0; i < clt->quant; i++)
    {
        if(id == clt->clientes[i].id) // Achou o id, cliente existe na lista
        {
            return i; // Retorna a posição do cliente na lista
        }
    }

    return -1; // Cliente não existe
}


int buscar_cpf( int id_cliente, ListaCpf *lcpf)
{
    // feita usando e adaptando a lógica de buscar_id_cliente
    int i;
    for(i = 0; i < lcpf->quant; i++)
    {
        if(id_cliente == lcpf->cpfs[i].id)
        {
            return i; // Retorna a posição do cpf do cliente
        }
    }

    return -1; // Cliente não existe
}

int buscar_cnpj( int id_cliente, ListaCnpj *lcnpj)
{
    // feita usando e adaptando a lógica de buscar_id_cliente
    int i;
    for(i = 0; i < lcnpj->quant; i++)
    {
        if(id_cliente == lcnpj->cnpjs[i].id)
        {
            return i; // Retorna a posição do cpf do cliente
        }
    }

    return -1; // Cliente não existe
}


// Função para verificar se o cpf já existe (faz parte de "Análisar cliente")

bool verificar_cpf(ListaCpf *clt, char *cpf_digitado)
{
    int i;
    for(i = 0; i < clt->quant; i++)
    {
        if(strcmp(cpf_digitado, clt->cpfs[i].cpf) == 0)
        {
            return false; // cpf já cadastrado
        }
    }
    return true;
}

// função para verificar se o cnpj já existe (faz parte de "Análisar cliente")

bool verificar_cnpj(ListaCnpj *clt, char *cnpj_digitado)
{
    int i;
    for(i = 0; i < clt->quant; i++)
    {
        if(strcmp(cnpj_digitado, clt->cnpjs[i].cnpj) == 0)
        {
            return false; // cnpj já cadastrado
        }
    }
    return true;
}

cliente* consultarCliente(ListaCliente *lclientes, int id) {
    /*
    Implementação do caso de uso "Consultar cliente"

    Retorna:
    - Valor do tipo cliente (Se tiver um cliente com id correspondente)
    - NULL (Se não tiver um cliente com id correspondente)

    Segui o mesmo padrão das outras funções, se estiver errado, por favor corrija ou me avise que está errado
    */

    /* Fiz algumas mudanças nessa função com o objetivo de otimizá-la:

       1 Troquei o parâmetro para um ponteiro para a lista ao invés da lista toda
       2 A função passa agora a retornar o endereço da lista de clientes
       3 Isso torna a função mais rápida, pois fazer uma cópia inteira da lista é desnecessário,
         basta apenas pega o endereço da lista original para pegar a informação necessária
         
         Assinado: Lucas
    */

    int i_cliente = buscar_id_cliente(lclientes, id);

    if(i_cliente != -1) {
        return &lclientes->clientes[i_cliente];
    }
    return NULL;
}

// função para inserir um cliente

bool InserirCliente(cliente novo_cliente, ListaCliente *clt)
{
    if (clt == NULL) // segurança
    {
        return false;
    }

    // verificar se o vetor de clientes está cheio
    if (clt->quant == clt->max)
    {
        // dobrar o tamanho do vetor (otimização)
        // se tiver 0 adotar o tamanho padrão 10

        int nova_capacidade = (clt->max == 0) ? 10 : clt->max * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(cliente);

        // realloc para aumentar o tamanho do vetor de clientes
        // usei um ponteiro temporario para armazenar a nova lista por motivos de segurança

        cliente* temp = realloc(clt->clientes, novo_tamanho_bytes);
        
        // ver se o realloc deu certo

        if(temp == NULL)
        {
            return false;
        }
        // se chegar aqui é porque deu certo pode usar o novo tamanho do vetor de clientes

        clt->clientes = temp;
        clt->max = nova_capacidade;

    }

    clt->clientes[clt->quant] = novo_cliente; // inserindo o novo cliente
    clt->quant++; // nova quantidade de clientes

    return true;
    
}

// quando o cliente digitar o cpf na interface chama essa função

bool InserirCpf(ListaCpf *clt, PessoaFisica cpf)
{
    if (clt == NULL) // segurança
    {
        return false;
    }

    // verificar se tem espaço na list

    if(clt->quant == clt->max)
    {
        // dobrar o tamanho do vetor (otimização)
        // se tiver 0 adotar o tamanho padrão 10

        int nova_capacidade = (clt->max == 0) ? 10 : clt->max * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(PessoaFisica);
        PessoaFisica* temp = realloc(clt->cpfs, novo_tamanho_bytes);

        // ver se o realloc deu certo

        if(temp == NULL)
        {
            return false;
        } 
        
        // se chegar aqui é porque deu certo pode usar o novo tamanho do vetor de cpfs

        clt->cpfs = temp;
        clt->max = nova_capacidade;

    }
    
    clt->cpfs[clt->quant] = cpf; // inserindo o novo cpf
    clt->quant++; // incrementando a quantidade

    return true;

}

// quando o cliente digitar o cnpj na interface chama essa função

bool InserirCnpj(ListaCnpj *clt, PessoaJuridica cnpj)
{
    if (clt == NULL) // segurança
    {
        return false;
    }

    // verificar se tem espaço na list

    if(clt->quant == clt->max)
    {
        // dobrar o tamanho do vetor (otimização)
        // se tiver 0 adotar o tamanho padrão 10

        int nova_capacidade = (clt->max == 0) ? 10 : clt->max * 2;
        int novo_tamanho_bytes = nova_capacidade * sizeof(PessoaJuridica);
        PessoaJuridica* temp = realloc(clt->cnpjs, novo_tamanho_bytes);

        // ver se o realloc deu certo

        if(temp == NULL)
        {
            return false;
        } 
        
        // se chegar aqui é porque deu certo pode usar o novo tamanho do vetor de cnpjs

        clt->cnpjs = temp;
        clt->max = nova_capacidade;

    }
    
    clt->cnpjs[clt->quant] = cnpj; // inserindo o novo cnpjs
    clt->quant++; // incrementando a quantidade

    return true;

}

bool removerCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj, int idremove){
    
    int indice;

    // usuario entra com o id do cliente e a funcao verifica se existe o cliente
    indice = buscar_id_cliente(clt, idremove); // mudando para nova função

    if(indice == -1){ // id não exsite
        return false;
    }

    // descobre se tem que tirar cpf ou cnpj

    TipoCliente tipo_remove = clt->clientes[indice].tipo;   

    for(int i=indice; i<(clt->quant - 1); i++){
        clt->clientes[i] = clt->clientes[i+1];
    }
    clt->quant--;

    //removendo cpfs e cnpjs

    if(tipo_remove == TIPO_PESSOA_FISICA)
    {
        int i_cpf = -1; // posição desse cliente na lista de cpfs

        for(int k = 0; k < cpf->quant; k++) {
            if (cpf->cpfs[k].id == idremove) { // Achou pelo ID
                i_cpf = k;
                break;
            }
        }
        if(i_cpf != -1) { // segurança
            for(int i=i_cpf; i<(cpf->quant - 1); i++){
                cpf->cpfs[i] = cpf->cpfs[i+1];
            }
            cpf->quant--;
        }
    }
    else
    {
        int i_cnpj = -1; // posição desse cliente na lista de cnpjs

        for(int k = 0; k < cnpj->quant; k++) {
            if (cnpj->cnpjs[k].id == idremove) {
                i_cnpj = k;
                break;
            }
        }
        if(i_cnpj != -1) { // segurança

            for(int i=i_cnpj; i<(cnpj->quant - 1); i++){
                cnpj->cnpjs[i] = cnpj->cnpjs[i+1];
            }
            cnpj->quant--;
        }
    }

    return true;
}


// função para validar cpf

bool Validar_CPF(char *cpf) 
{
    // --- 1. LIMPEZA DOS CARACTERES ESPECIAIS (FILTRAGEM IN-PLACE) ---
    int i = 0; // Índice de leitura
    int j = 0; // Índice de escrita

    while (cpf[i] != '\0') 
    {
        if (isdigit((unsigned char)cpf[i])) 
        {
            cpf[j] = cpf[i];
            j++; 
        }
        i++;
    }
    cpf[j] = '\0'; // Finaliza a string limpa

    // --- 2. PRÉ-VALIDAÇÃO DE TAMANHO ---

    if (strlen(cpf) != 11) 
    {
        return false; 
    }

    // --- 3. PRÉ-VALIDAÇÃO DE DÍGITOS REPETIDOS ---
    char primeiro_caractere = cpf[0];
    bool todos_iguais = true; // Assume que são iguais até provar o contrário

    for (int k = 1; k < 11; k++) 
    {
        if (cpf[k] != primeiro_caractere) 
        {
            todos_iguais = false;
            break; // Sai do loop assim que encontrar o primeiro diferente
        }
    }
    
    // Se 'todos_iguais' for true, o CPF é inválido

    if (todos_iguais)
    {
        return false;
    }
    
    // --- 4. CÁLCULO E VERIFICAÇÃO DO PRIMEIRO DÍGITO VERIFICADOR (DV1 - 10º dígito) ---
    
    int soma_dv1 = 0;
    int peso = 10;
    
    // Multiplica os 9 primeiros dígitos por pesos decrescentes (10 a 2)
    for(int k = 0; k < 9; k++)
    {
        // Diminui '0' para converter o char (ex: '5') para o int (ex: 5)
        soma_dv1 += (cpf[k] - '0') * peso;
        peso--; 
    }

    int resto_dv1 = soma_dv1 % 11;
    int dv1_calculado;
    
    if(resto_dv1 < 2) 
    {
        dv1_calculado = 0;
    } 
    else 
    {
        dv1_calculado = 11 - resto_dv1;
    }

    if(dv1_calculado != (cpf[9] - '0'))
    {
        return false;
    }

    // --- 5. CÁLCULO E VERIFICAÇÃO DO SEGUNDO DÍGITO VERIFICADOR (DV2 - 11º dígito) ---
    
    int soma_dv2 = 0;
    peso = 11; // O peso começa em 11 e multiplica os 10 primeiros dígitos (incluindo o DV1)
    
    // Multiplica os 10 primeiros dígitos por pesos decrescentes (11 a 2)
    for(int k = 0; k < 10; k++) 
    {
        soma_dv2 += (cpf[k] - '0') * peso;
        peso--;
    }

    int resto_dv2 = soma_dv2 % 11;
    int dv2_calculado;
    
    if(resto_dv2 < 2) 
    {
        dv2_calculado = 0;
    } 
    else 
    {
        dv2_calculado = 11 - resto_dv2;
    }

    if(dv2_calculado != (cpf[10] - '0'))
    {
        return false;
    }

    // Se passou por todas as verificações, o CPF é válido!

    return true;
}

// função para validar cnpj

bool Validar_CNPJ(char *cnpj) 
{
    // --- 1. LIMPEZA DOS CARACTERES ESPECIAIS (FILTRAGEM IN-PLACE) ---
    int i = 0; // Índice de leitura
    int j = 0; // Índice de escrita

    while (cnpj[i] != '\0') 
    {
        if (isdigit((unsigned char)cnpj[i])) 
        {
            cnpj[j] = cnpj[i];
            j++; 
        }
        i++;
    }
    cnpj[j] = '\0'; // Finaliza a string limpa

    // --- 2. PRÉ-VALIDAÇÃO DE TAMANHO ---

    if (strlen(cnpj) != 14) 
    {
        return false; 
    }

    // --- 3. PRÉ-VALIDAÇÃO DE DÍGITOS REPETIDOS ---
    char primeiro_caractere = cnpj[0];
    bool todos_iguais = true; 

    // O loop vai até o 14º dígito (índice 13)
    for (int k = 1; k < 14; k++) 
    {
        if (cnpj[k] != primeiro_caractere) 
        {
            todos_iguais = false;
            break; 
        }
    }
    
    if (todos_iguais)
    {
        return false;
    }
    
    // --------------------------------------------------------------------------
    // --- 4. CÁLCULO E VERIFICAÇÃO DO PRIMEIRO DÍGITO VERIFICADOR (DV1 - 13º dígito) ---
    // --------------------------------------------------------------------------
    
    int soma_dv1 = 0;
    // A sequência de pesos é cíclica: 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2
    int pesos_dv1[12] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2}; 
    
    // Multiplica os 12 primeiros dígitos (índices 0 a 11)
    for(int k = 0; k < 12; k++)
    {
        // Posição do peso na sequência: 
        // 12 - 1 - k. Ex: k=0 (1º dígito) usa peso[11], k=11 (12º dígito) usa peso[0].
        // Melhor: usar um array de 12 elementos e iterar do fim para o começo.
        
        // Itera sobre os dígitos de 0 a 11 e usa o peso correspondente.
        // O peso 5 é para o dígito 1, o peso 2 é para o dígito 12.
        
        // A lógica de peso_dv1[k] é mais fácil de implementar.
        soma_dv1 += (cnpj[k] - '0') * pesos_dv1[k];
    }
    
    int resto_dv1 = soma_dv1 % 11;
    int dv1_calculado;
    
    if(resto_dv1 < 2) 
    {
        dv1_calculado = 0;
    } 
    else 
    {
        dv1_calculado = 11 - resto_dv1;
    }

    // O DV1 é o 13º dígito, que fica no índice 12 (0 a 13 são 14 dígitos).
    if(dv1_calculado != (cnpj[12] - '0'))
    {
        return false;
    }

    // --------------------------------------------------------------------------
    // --- 5. CÁLCULO E VERIFICAÇÃO DO SEGUNDO DÍGITO VERIFICADOR (DV2 - 14º dígito) ---
    // --------------------------------------------------------------------------
    
    int soma_dv2 = 0;
    // A sequência de pesos é cíclica: 6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2
    int pesos_dv2[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2}; 
    
    // Multiplica os 13 primeiros dígitos (incluindo o DV1 calculado, índices 0 a 12)
    for(int k = 0; k < 13; k++) 
    {
        soma_dv2 += (cnpj[k] - '0') * pesos_dv2[k];
    }

    int resto_dv2 = soma_dv2 % 11;
    int dv2_calculado;
    
    if(resto_dv2 < 2) 
    {
        dv2_calculado = 0;
    } 
    else 
    {
        dv2_calculado = 11 - resto_dv2;
    }

    // O DV2 é o 14º dígito, que fica no índice 13.
    if(dv2_calculado != (cnpj[13] - '0'))
    {
        return false;
    }

    return true;
}
