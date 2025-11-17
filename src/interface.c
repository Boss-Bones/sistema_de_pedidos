#include "interface.h"
#include "cliente.h"
#include "pedido.h"
#include "persistencia.h"
#include "produto.h"
#include <ncurses.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h> // Ainda vou usar

/*
Obs importânte: O editor de código pode mostrar erros em relação a tipos desconhecidos, pórem ao finalizar todo o projeto, os erros irão sumir
*/

// =========== Funções auxiliares ===========

void printTituloJanela(WINDOW* janela, char titulo[50], int t) {
    /*
    Função para imprimir o titulo da janela

    janela -> Referênte a janela que estará o título
    titulo -> Título da janela
    t -> Tamanho do título
    */

    mvwaddch(janela, 0, 2, ACS_RTEE);
    wattron(janela, A_BOLD);
    mvwprintw(janela, 0, 3, " %s ", titulo);
    wattroff(janela, A_BOLD);
    mvwaddch(janela, 0, 5+t, ACS_LTEE);

}

void printOpcoesJanela(WINDOW* janela, char opcoes[][50], int q) {
    /*
    Função para imprimir as opções da janela

    janela -> Referênte a janela que estará o título
    opcoes -> Opções da janela
    q -> Quantidade de opções
    */

    int h, w;
    getmaxyx(janela, h, w);
    h--; w--;

    mvwaddch(janela, h, 2, ACS_RTEE);
    mvwaddch(janela, h, 3, ' ');
    wmove(janela, h, 4);
    for(int i = 0; i < q; i++) {
        if((i%2)-1) {
            wattron(janela, COLOR_PAIR(1));
        }

        wprintw(janela, "%s", opcoes[i]);

        if((i%2)-1) {
            wattroff(janela, COLOR_PAIR(1));
        } else {
            waddch(janela, ' ');
            if(i+1 < q) {
                if(i > 1) {
                    waddch(janela, ACS_VLINE);
                } else {
                    waddch(janela, ACS_LTEE);
                    waddch(janela, ACS_HLINE);
                    waddch(janela, ACS_RTEE);
                }
            }
        }

        if(i+1 < q) {
            waddch(janela, ' ');
        }
    }
    waddch(janela, ACS_LTEE);

}

void printPLinhaUni(WINDOW* janela, int y, int x, int c, char tags[][50], int q) {
    /*
    Função para fazer a primeira linha da tabela, essa linha terá tags de qualqer seja a struct

    Os argumentos são:
    janela -> Referênte a janela que a linha estará
    y -> Localização do inicio em y
    x -> Localização do inicio em x
    c -> Comprimento, começa em (x,y) e termina em (x+c-1,y)
    tags -> Tags da primeira linha
    q -> Quantidade de tags
    */

    wattron(janela, COLOR_PAIR(1));
    wattron(janela, A_BOLD);
    mvwhline(janela, y, x, ' ', c);
    for(int i = 0; i < q; i++) {
        mvwprintw(janela, y, x+(c/q)*i, "%s", tags[i]);
    }
    wattroff(janela, COLOR_PAIR(1));
    wattroff(janela, A_BOLD);
}

void printLinhaPedido(WINDOW* janela, int y, int x, int c, pedido pd) {
    /*
    Função para fazer a linha da tabela, essa linha terá as informações do pedido

    Os argumentos são:
    janela -> Referênte a janela que a linha estará
    y -> Localização do inicio em y
    x -> Localização do inicio em x
    c -> Comprimento, começa em (x,y) e termina em (x+c-1,y)
    pd -> Pedido individual, o conteúdo da linha
    */

    mvwhline(janela, y, x, ' ', c);
    mvwprintw(janela, y, x+((c)/4)*0, "%d", pd.id);
    mvwprintw(janela, y, x+((c)/4)*1, "%d", pd.clienteId);
    mvwprintw(janela, y, x+((c)/4)*2, "%s", pd.data);
    mvwprintw(janela, y, x+((c)/4)*3, "%.2lf reais", pd.total);
}

// Inspiração para criação de mais listas
void mostrarListaPedidos(WINDOW* janela, int y, ListaPedido* lp) {
    int hj, wj; getmaxyx(janela, hj, wj); // Dimensões da janela
    char tags[4][50] = {"ID do pedido", "ID do cliente", "Data", "Total"}; // Tags da lista

    // Linha das tags
    printPLinhaUni(janela, y, 2, wj-4, tags, 4);
    // e as demais linhas, que já são os itens em si
    for(int i = 0; i < (*lp).quant && (y+1)+i < hj-3; i++) {
        printLinhaPedido(janela, y+1+i, 2, wj-4, *((*lp).pedidos+i));
    }
    if((*lp).quant > (hj-3)-(y+1)) {
        mvwprintw(janela, hj-3, 2, "...");
    }
    // ..., a primeira linha está selecionada
    wattron(janela, COLOR_PAIR(2));
    printLinhaPedido(janela, y+1, 2, wj-4, *((*lp).pedidos));
    wattroff(janela, COLOR_PAIR(2));
    // Atualizando a janela
    refresh();
    wrefresh(janela);

    // Deslocando
    int ini=y+1, sel=0, sely=ini, entr; // Y do inicio da lista; Seleção; Y da seleção; Entrada do teclado
    while(true) {
        entr = wgetch(janela);
        switch(entr) {
            case KEY_UP:
                if(!(sely == y+1 && sel > 0)) { // Ainda não é o primeiro aparecendo visualmente
                    if(sel) { // Não sendo 0, ainda dá para subir
                        // A linha selecionada perde a seleção
                        printLinhaPedido(janela, sely, 2, wj-4, *((*lp).pedidos+sel));
                        
                        // Muda a seleção
                        sel--;
                        sely--;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaPedido(janela, sely, 2, wj-4, *((*lp).pedidos+sel));
                        wattroff(janela, COLOR_PAIR(2));

                        //Atualiza
                        refresh();
                        wrefresh(janela);
                    }
                } else {
                    // Diminui na seleção
                    sel--;

                    // Scrolla para cima
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaPedido(janela, y+1, 2, wj-4, *((*lp).pedidos+sel));
                    wattroff(janela, COLOR_PAIR(2));
                    for(int i = 1; i < (hj-3)-(y+1); i++) {
                        printLinhaPedido(janela, y+1+i, 2, wj-4, *((*lp).pedidos+sel+i));
                    }

                    // Ao scrollar, os últimos conteúdos sumiriam, voltar os 3 pontinhos
                    mvwprintw(janela, hj-3, 2, "...");

                    // Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case KEY_DOWN:
                if(!(sely == hj-4 && sel < (*lp).quant-1)) { // Ainda não é o último aparecendo visualmente
                    if(sel < (*lp).quant-1) { // Ainda menor que o último índice, ainda dá para descer
                        // A linha selecionada perde a seleção
                        printLinhaPedido(janela, sely, 2, wj-4, *((*lp).pedidos+sel));

                        // Muda a seleção
                        sel++;
                        sely++;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaPedido(janela, sely, 2, wj-4, *((*lp).pedidos+sel));
                        wattroff(janela, COLOR_PAIR(2));

                        //Atualiza
                        refresh();
                        wrefresh(janela);
                    }
                } else {
                    // Adiciona na seleção
                    sel++;

                    // Scrolla para baixo
                    for(int i = 0; i < (hj-3)-(y+1)-1; i++) {
                        printLinhaPedido(janela, y+1+i, 2, wj-4, *((*lp).pedidos+sel-((hj-3)-(y+1)-1)+i));
                    }
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaPedido(janela, hj-4, 2, wj-4, *((*lp).pedidos+sel));
                    wattroff(janela, COLOR_PAIR(2));

                    // Se chegar ao final, tirar os 3 pontinhos
                    if(sel == (*lp).quant-1) {
                        mvwprintw(janela, hj-3, 2, "   ");
                    }

                    //Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case 'q':
                delwin(janela);
                clear();
                refresh();
                return;
                break;
        }
    }
}

void criarCaixa(WINDOW* janela, int y, int x, int c, int a) {
    /*
    Função para criar uma caixa interna a janela

    Os argumentos são:
    janela -> Referênte a janela que a linha estará
    y -> Localização do inicio em y
    x -> Localização do inicio em x
    c -> Comprimento, começa em (x,y) e termina em (x+c-1,y)
    pd -> Pedido individual, o conteúdo da linha
    */

    // Linhas horizontais
    mvwhline(janela, y, x, ACS_HLINE, c);
    mvwhline(janela, y+a+1, x, ACS_HLINE, c);

    // Linhas verticais
    mvwvline(janela, y+1, x, ACS_VLINE, a);
    mvwvline(janela, y+1, x+c-1, ACS_VLINE, a);

    // Cantos
    mvwaddch(janela, y, x, ACS_ULCORNER);
    mvwaddch(janela, y, x+c-1, ACS_URCORNER);
    mvwaddch(janela, y+a+1, x, ACS_LLCORNER);
    mvwaddch(janela, y+a+1, x+c-1, ACS_LRCORNER);
}

// Ainda não utilizado, não use ainda
void ligarBlur(WINDOW* janela) {
    int y, x;
    getmaxyx(janela, y, x);

    for(int i = 0; i < y; i++) {
        for(int j = 0; j < x; j++) {
            if(mvwinch(janela, i, j) != ' ') {
                mvwaddch(janela, i, j, '.');
            }
        }
    }

    refresh();
    wrefresh(janela);
}

// =========== Sub-menus ===========

/* Vazio ainda */

// =========== Menus ===========

void mostrarMenuPedidos(ListaPedido* lp) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10, wj = w-w/10; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Menu de pedidos"; // Título da janela
    char opcoes[6][50] = {"q", "Voltar", "a", "Adicionar", "r", "Remover"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 15); // Título
    printOpcoesJanela(janela, opcoes, 6);

    /*
    Parei meu trabalho por aqui, ainda há coisas a serem adicionadas
    */
    mvwprintw(janela, 1, 1, " v Lista de pedidos"); // Vai funcionar como gaveta, por enquanto a lista aparece diretamente
    mostrarListaPedidos(janela, 5, lp);

    
    getch();
}

void mostrarMenuPrincipal(ListaPedido* lp) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/3, wj = w-w/2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char opcoes[4][50] = {"Clientes", "Produtos", "Pedidos", "Sair"}; // Opções
    int t_op[4] = {8, 8, 8, 4}; // Tamanho das strings (Usar para centralização)

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão

    // Desenhando o título
    mvwprintw(janela, hj/4-2, wj/2-10, "  ___    ___   ___ ");
    mvwprintw(janela, hj/4-1, wj/2-10, " / __|  / __| | _ |");
    mvwprintw(janela, hj/4, wj/2-10,   " |__ | | (__  |  _/");
    mvwprintw(janela, hj/4+1, wj/2-10, " |___/  |___| |_|  ");

    // Caixa de opções
    criarCaixa(janela, hj/2, 4, wj-8, 4);
    // e as opções
    wattron(janela, COLOR_PAIR(2));
    mvwhline(janela, hj/2+1, 5, ' ', wj-10);
    mvwprintw(janela, hj/2+1, wj/2-(t_op[0]/2), "%s", opcoes[0]);
    wattroff(janela, COLOR_PAIR(2));
    for(int i = 1; i < 3; i++) {
        mvwprintw(janela, hj/2+1+i, wj/2-(t_op[i]/2), "%s", opcoes[i]);
    }
    wattron(janela, COLOR_PAIR(4));
    wattron(janela, A_BOLD);
    mvwprintw(janela, hj/2+1+3, wj/2-(t_op[3]/2), "%s", opcoes[3]);
    wattroff(janela, COLOR_PAIR(4));
    wattroff(janela, A_BOLD);

    refresh();
    wrefresh(janela);

    // Loop de receber entrada
    int sel = 0, entr; // Seleção atual
    while(true) {
        entr = wgetch(janela);
        //ligarBlur(janela);

        switch(entr)
        {
            case KEY_UP:
                if(sel) {
                    if(sel == 3) {
                        wattron(janela, COLOR_PAIR(4));
                        wattron(janela, A_BOLD);
                    }
                    mvwhline(janela, hj/2+1+sel, 5, ' ', wj-10); 
                    mvwprintw(janela, hj/2+1+sel, wj/2-(t_op[sel]/2), "%s", opcoes[sel]);
                    if(sel == 3) {
                        wattroff(janela, COLOR_PAIR(4));
                        wattroff(janela, A_BOLD);
                    }
                    sel--;
                    wattron(janela, COLOR_PAIR(2));
                    mvwhline(janela, hj/2+1+sel, 5, ' ', wj-10);
                    mvwprintw(janela, hj/2+1+sel, wj/2-(t_op[sel]/2), "%s", opcoes[sel]);
                    wattroff(janela, COLOR_PAIR(2));

                    refresh();
                    wrefresh(janela);
                }
                break;
            case KEY_DOWN:
                if(sel < 3) {
                    mvwhline(janela, hj/2+1+sel, 5, ' ', wj-10); 
                    mvwprintw(janela, hj/2+1+sel, wj/2-(t_op[sel]/2), "%s", opcoes[sel]);
                    sel++;
                    if(sel == 3)
                        wattron(janela, COLOR_PAIR(3));
                    else
                        wattron(janela, COLOR_PAIR(2));
                    mvwhline(janela, hj/2+1+sel, 5, ' ', wj-10);
                    mvwprintw(janela, hj/2+1+sel, wj/2-(t_op[sel]/2), "%s", opcoes[sel]);
                    if(sel == 3)
                        wattroff(janela, COLOR_PAIR(3));
                    else
                        wattroff(janela, COLOR_PAIR(2));

                    refresh();
                    wrefresh(janela);
                }
                break;
            case 10:
                switch(sel)
                {
                    case 0: // Cliente
                        break;
                    case 1: // Produto
                        break;
                    case 2: // Pedido
                        mostrarMenuPedidos(lp);
                        break;
                    case 3: // Fechar a aplicação
                        return;
                        break;
                }
        }
    }

    // Versão do sistema
    //wattron(janela, COLOR_PAIR(3));
    //mvwprintw(janela, hj-2, (wj-1)-5, "Alpha");
    //wattroff(janela, COLOR_PAIR(3));

    refresh();
    wrefresh(janela);
    getch();
}

// =========== Anotações do dev ===========

/*
Funções que devo criar:

void mostrarMenuPrincipal(){}

void mostrarMenuClientes(){}

void mostrarMenuProdutos(){}

void mostrarMenuPedidos(){}

*/