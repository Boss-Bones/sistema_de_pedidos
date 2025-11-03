#include "cliente.h"

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

// Parte Clebio

#include "cliente.h"

void inserirCliente(){}

void listarCliente(){}

void editarCliente(){}

void removerCliente(){}

void salvarClienteCSV(){}

void carregarClienteCSV(){}
