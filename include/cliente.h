#ifndef CLIENTE 
#define CLIENTE
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Coloque a prototipação da função aqui

bool InserirCliente(cliente novo_cliente, ListaCliente *clt);

bool verificar_id_cliente(ListaCliente *clt, int id);

int buscar_id_cliente(ListaCliente* clt, int id);

bool verificar_cpf(ListaCpf *clt, char *cpf_digitado);

bool verificar_cnpj(ListaCnpj *clt, char *cnpj_digitado);

cliente* consultarCliente(ListaCliente *lclientes, int id);

bool InserirCpf(ListaCpf *clt, PessoaFisica cpf);

void listarCliente();

void editarCliente();

bool removerCliente(ListaCliente *clt, int idremove);

void salvarClienteCSV();

void carregarClienteCSV();

bool Validar_CPF(char *cpf);

bool Validar_CNPJ(char *cnpj);
#endif