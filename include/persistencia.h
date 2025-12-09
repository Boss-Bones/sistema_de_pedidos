#ifndef PERSISTENCIA 
#define PERSISTENCIA

#include "cliente.h"
#include "produto.h"
#include "pedido.h"

// Coloque a prototipação da função aqui
int salvarCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj);
int carregarCliente(ListaCliente *clt, ListaCpf *cpf, ListaCnpj *cnpj);
int salvarProduto(ListaProduto *pdt);
int carregarProduto(ListaProduto *pdt);
int salvarPedido(ListaPedido *pdd);
int salvarItemPedido(ListaPedido *pdd);
int carregarPedido(ListaPedido *pdd);
int carregarItemPedido(ListaPedido *pdd);

#endif