#ifndef CLIENTE 
#define CLIENTE
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Coloque a prototipação da função aqui

bool InserirCliente(cliente novo_cliente, ListaCliente *clt);

bool verificar_id_cliente(ListaCliente *clt, int id);

int buscar_id_cliente(ListaCliente* clt, int id);

int buscar_cpf( int cpf, ListaCpf *lcpf);

int buscar_cnpj( int cnpj, ListaCnpj *lcnpj);

bool verificar_cpf(ListaCpf *clt, char *cpf_digitado);

bool verificar_cnpj(ListaCnpj *clt, char *cnpj_digitado);

cliente* consultarCliente(ListaCliente *lclientes, int id);

bool InserirCpf(ListaCpf *clt, PessoaFisica cpf);

bool InserirCnpj(ListaCnpj *clt, PessoaJuridica cnpj);

void listarCliente();

void editarCliente();

bool removerCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj, int idremove);

bool Validar_CPF(char *cpf);

bool Validar_CNPJ(char *cnpj);
#endif