#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include "cliente.h"
#include "interface.h"
#include "pedido.h"
#include "persistencia.h"
#include "produto.h"

int main() {
    // Definindo idioma (A impressão dos valores passam de . para ,)
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Lista de pedidos
    ListaPedido* ls = (ListaPedido*)malloc(sizeof(ListaPedido));
    (*ls).pedidos = NULL;
    (*ls).max = 0;
    (*ls).quant = 0;
    // Obtendo os arquivos
    int retorno_p = carregarPedido(ls);
    int retorno_ip = carregarItemPedido(ls);

    // Lista de clientes
    ListaCliente* lc = (ListaCliente*)malloc(sizeof(ListaCliente));
    (*lc).clientes = NULL;
    (*lc).max = 0;
    (*lc).quant = 0;
    // Lista de CPF
    ListaCpf* lcp = (ListaCpf*)malloc(sizeof(ListaCpf));
    (*lcp).cpfs = NULL;
    (*lcp).max = 0;
    (*lcp).quant = 0;
    // Lista de CNPJ
    ListaCnpj* lcn = (ListaCnpj*)malloc(sizeof(ListaCnpj));
    (*lcn).cnpjs = NULL;
    (*lcn).max = 0;
    (*lcn).quant = 0;
    // Obtendo dos arquivos
    int retorno_c = carregarCliente(lc, lcp, lcn);

    // Lista de produtos
    ListaProduto* lpr = (ListaProduto*)malloc(sizeof(ListaProduto));
    (*lpr).produtos = NULL;
    (*lpr).max = 0;
    (*lpr).quant = 0;
    // Obtendo os arquivos
    int retorno_pr = carregarProduto(lpr);

    // Inicializando a tela
    initscr();
    start_color(); // Uso de cores
    curs_set(0); // Cursor não aparece
    noecho(); // Sem eco
    set_escdelay(0); // Sem delay do ESC
    cbreak();
    init_pair(1, COLOR_BLACK, COLOR_WHITE); // 1º conjunto de cores | INANIMADO
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // 2º conjunto de cores | SELECIONADO
    init_pair(3, COLOR_WHITE, COLOR_RED); // 3º conjunto de cores | SELECIONADO_VERMELHO
    init_pair(4, COLOR_RED, COLOR_BLACK); // 4º conjunto de cores | VERMELHO_ERRO
    init_pair(5, COLOR_YELLOW, COLOR_BLACK); // 5º conjunto de cores | AMARELO_AVISO

    // Mostrando menu principal
    if(retorno_c || retorno_ip || retorno_p || retorno_pr) {
        mostrarAlerta(false, "Alguns arquivos estão ausentes, isso pode ocorrer se for o primeiro uso, esses arquivos serão automaticamente criados ao sair do programa.\n\nAperte qualquer tecla para continuar...");
    }
    if(getmaxy(stdscr) < 26 || getmaxx(stdscr) < 88) {
        mostrarAlerta(false, "O programa não foi projetado para telas menores que 88x26, tente usar em uma tela de terminal maior.\n\nAperte qualquer tecla para encerrar...");

        endwin();
        return 0;
    }
    mostrarMenuPrincipal(ls, lc, lpr, lcp, lcn);

    // Quando o menu principal for encerrado, tudo se encerra
    endwin();
    int ret_ped = salvarPedido(ls);
    int ret_iped = salvarItemPedido(ls);
    int ret_clt = salvarCliente(lc, lcp, lcn);
    int ret_prod = salvarProduto(lpr);
    if(ret_ped || ret_iped || ret_clt || ret_prod) {
        mostrarAlerta(false, "Alguns arquivos estão ausentes, isso pode ocorrer se for o primeiro uso, esses arquivos serão automaticamente criados ao sair do programa.\n\nAperte qualquer tecla para continuar...");
    }

    return 0;
}