#ifndef INTERFACE 
#define INTERFACE

#include <stdbool.h>
#include <ncurses.h>
#include "cliente.h"
#include "pedido.h"
#include "produto.h"

void printTituloJanela(WINDOW* janela, char titulo[50], int t);
void printOpcoesJanela(WINDOW* janela, char opcoes[][50], int q);
void printTexto(WINDOW* janela, int y, int x, char texto[]);
void printPadrao(WINDOW* janela, int y, int x, char texto[]);
void printTextoLim(WINDOW* janela, int y, int x, char texto[], int q);
void printTituloBasico(WINDOW* janela, int y, int x, char texto[]);
int scanOpcoes(WINDOW* janela, int y, char titulo[], char opcoes[][50], int q);
int scanNum(WINDOW* janela, int y, char titulo[]);
double scanFloat(WINDOW* janela, int y, char titulo[]);
int scanText(WINDOW* janela, int y, char titulo[], int max, char* dest);
int scanTextCPF(WINDOW* janela, int y, char* dest);
int scanTextCNPJ(WINDOW* janela, int y, char* dest);
void printPLinhaUni(WINDOW* janela, int y, int x, int c, char tags[][50], int q);
void printLinhaPedido(WINDOW* janela, int y, int x, int c, pedido pd);
void printLinhaProduto(WINDOW* janela, int y, int x, int c, Produto* prd);
void printLinhaCliente(WINDOW* janela, int y, int x, int c, cliente cl);
void mostrarListaPedidos(WINDOW* janela, int y, ListaPedido* lp, ListaProduto* lpr, int** janela_backup);
void mostrarListaProdutos(WINDOW* janela, int y, ListaProduto* lpr);
void mostrarListaClientes(WINDOW* janela, int y, ListaCliente* clt, int** janela_backup, ListaCpf* cpfs, ListaCnpj* cnpjs);
void criarCaixa(WINDOW* janela, int y, int x, int c, int a);
void ligarBlur(WINDOW* janela, int** janela_backup);
void desligarBlur(WINDOW* janela, int** janela_backup);
int mostrarOpcoesMenuUni(WINDOW* janela, char opcoes[][50], int q, int pre_s);
void mostrarAlerta(bool critico, char texto[]);
void mostrarSubSubMenuRemoverItem(pedido* p);
void mostrarSubSubMenuCadastrarItem(ListaProduto* lpr, pedido* p);
void mostrarSubSubMenuGerenciarItPedido(ListaProduto* lpr, pedido* pd);
void mostrarSubSubMenuMostrarItPedido(pedido* pd);
void mostrarCliente(cliente* cl, ListaCpf* cpfs, ListaCnpj* cnpjs);
void mostrarSubMenuCadastrarPedido(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr);
void mostrarSubMenuCadastrarProduto(ListaProduto* lpr);
void mostrarSubMenuCadastrarCliente(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn);
void mostrarSubMenuRemoverProduto(ListaProduto* lpr, ListaPedido* lp);
void mostrarSubMenuRemoverCliente(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn);
void mostrarSubMenuConsultarProduto(ListaProduto* lpr);
void mostrarSubMenuConsultarPedido(ListaPedido* lp);
void mostrarSubMenuConsultarCliente(ListaCliente* clt, ListaCpf* cpfs, ListaCnpj* cnpjs);
void mostrarMenuClientes(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn);
void mostrarMenuProdutos(ListaProduto* lpr, ListaPedido* lp);
void mostrarMenuPedidos(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr);
void mostrarMenuPrincipal(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr, ListaCpf* lcp, ListaCnpj* lcn);

#endif