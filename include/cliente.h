#ifndef CLIENTE 
#define CLIENTE
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Definir números para cpf e cnpj
typedef enum 
{
    TIPO_PESSOA_FISICA, // Valor 0
    TIPO_PESSOA_JURIDICA  // Valor 1

} TipoCliente;

typedef struct cliente
{
   // Representa um cliente do sistema.

    int id; // Um número de identificação único para cada cliente.
    char nome[100]; // O nome do cliente.
    char endereco[200]; // O endereço do cliente.
    char telefone[20]; // O número de telefone do cliente.
    TipoCliente tipo; // O tipo de cliente (pessoa física ou jurídica)

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

// lista de clientes

typedef struct
{   // documentação análoga à ListaProduto. Para consultar verifique a branch produto

    int quant; // Quantidade atual de clientes
    int max; // Quantidade máxima a ser armazenada
    cliente* clientes; // Ponteiro para structs de cliente

} ListaCliente;

// lista de cpfs

typedef struct
{   // documentação análoga à ListaProduto. Para consultar verifique a branch produto

    int quant; // Quantidade atual de cpfs
    int max; // Quantidade máxima a ser armazenada
    PessoaFisica* cpfs; // Ponteiro para structs de cpf

} ListaCpf;

// lista de cnpjs

typedef struct
{   // documentação análoga à ListaProduto. Para consultar verifique a branch produto

    int quant; // Quantidade atual de cnpjs
    int max; // Quantidade máxima a ser armazenada
    PessoaJuridica* cnpjs; // Ponteiro para structs de cnpj

} ListaCnpj;

// Coloque a prototipação da função aqui

bool InserirCliente(cliente novo_cliente, ListaCliente *clt);

bool verificar_id_cliente(ListaCliente *clt, int id);

int buscar_id_cliente(ListaCliente* clt, int id);

int buscar_cpf( int id_cliente, ListaCpf *lcpf);

int buscar_cnpj( int id_cliente, ListaCnpj *lcnpj);

bool verificar_cpf(ListaCpf *clt, char *cpf_digitado);

bool verificar_cnpj(ListaCnpj *clt, char *cnpj_digitado);

cliente* consultarCliente(ListaCliente *lclientes, int id);

bool InserirCpf(ListaCpf *clt, PessoaFisica cpf);

bool InserirCnpj(ListaCnpj *clt, PessoaJuridica cnpj);

bool removerCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj, int idremove);

bool Validar_CPF(char *cpf);

bool Validar_CNPJ(char *cnpj);
#endif