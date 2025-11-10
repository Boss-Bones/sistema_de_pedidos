#include "cliente.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct cliente
{
   // Representa um cliente do sistema.

    int id; // Um número de identificação único para cada cliente.
    char nome[100]; // O nome do cliente.
    char endereco[200]; // O endereço do cliente.
    char telefone[20]; // O número de telefone do cliente.

} cliente;

typedef struct PessoaFisica
{

    // Representa um cliente pessoa física (CPF)

    int id; // Mesmo número usado na struct cliente
    char cpf[15]; // Char para incluir os digitos numéricos e especiais como o '-'

} PessoaFisica;

typedef struct PessoaJuridica
{
    // Representa um cliente pessoa jurídica (CNPJ)

    int id; // Mesmo número usado na struct cliente
    char cnpj[19]; // Char para incluir os digitos numéricos e especiais como o '-'

} PessoaJuridica;


void inserirCliente(){}

void listarCliente(){}

void editarCliente(){}

bool removerCliente(ListaCliente *clt, int idremove){
    
    int indice;

    // usuario entra com o id do cliente e a funcao verifica se existe o cliente
    indice = verificarCliente(idremove);
    if(indice == -1){
        return false;
    }

    for(int i=indice; i<(clt->quant - 1); i++){
        clt->clientes[i] = clt->clientes[i+1];
    }
    clt->quant--;

    return true;
}


void salvarClienteCSV(){}

void carregarClienteCSV(){}

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
