#include "interface.h"
#include "cliente.h"
#include "pedido.h"
#include "persistencia.h"
#include "produto.h"
#include <ncurses.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h> // Ainda vou usar
#include <string.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>

/*
Obs importânte: O editor de código pode mostrar erros em relação a tipos desconhecidos, pórem ao finalizar todo o projeto, os erros irão sumir
*/

// =========== Funções auxiliares ===========

// Funções de saída
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

void printTexto(WINDOW* janela, int y, int x, char texto[]) {
    wmove(janela, y, x);

    char palavra[100];
    int j = 0;
    for(int i = 0; texto[i] != '\0'; i++) {
        if(texto[i] == ' ' || texto[i] == '\n') {
            palavra[j] = '\0';
            j = 0;

            if(strlen(palavra)+getcurx(janela) < (double)getmaxx(janela)-2) {
                wprintw(janela, "%s ", palavra);
            } else {
                wmove(janela, getcury(janela)+1, x);
                wprintw(janela, "%s ", palavra);
            }

            if(texto[i] == '\n') {
                wmove(janela, getcury(janela)+1, x);
            }
        } else {
            palavra[j] = texto[i];
            j++;
        }
    }
    palavra[j] = '\0';
    if(strlen(palavra)+getcurx(janela) < (double)getmaxx(janela)-2) {
        wprintw(janela, "%s ", palavra);
    } else {
        wmove(janela, getcury(janela)+1, x);
        wprintw(janela, "%s ", palavra);
    }
}

void printPadrao(WINDOW* janela, int y, int x, char texto[]) {
    int curs = getcurx(janela);

    for(int i = 0; texto[i] != '\0'; i++) {
        if(texto[i] != ' ') {
            mvwaddch(janela, y, x+i, texto[i]);
        }
    }

    wmove(janela, y, curs);
}

void printTextoLim(WINDOW* janela, int y, int x, char texto[], int q) {
    // Váriavel q é o máximo do texto
    char temp[strlen(texto)];
    
    if(strlen(texto) < (double)q) {
        mvwprintw(janela, y, x, "%s", texto);
    } else {
        strcpy(temp, texto);
        temp[q-1] = '\0';
        temp[q-2] = '.';
        temp[q-3] = '.';
        temp[q-4] = '.';
        mvwprintw(janela, y, x, "%s", temp);
    }
}

void printTituloBasico(WINDOW* janela, int y, int x, char texto[]) {
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, x, "%s", texto);
    wattroff(janela, A_BOLD);
}

// Funções de entrada
int scanOpcoes(WINDOW* janela, int y, char titulo[], char opcoes[][50], int q) {
    /*
    Função para receber entrada, essa em específica é de acordo com a opção marcasa

    janela -> Referênte a janela que estará isso
    opcoes -> Opções
    q -> Quantidade de opções

    Se retorna -1, é que saiu do campo de digitação com 'esc'
    */

    // Título
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "%s", titulo);
    wattroff(janela, A_BOLD);

    // Opções
    wattron(janela, COLOR_PAIR(2));
    mvwprintw(janela, y+1, 2, "%s", opcoes[0]);
    wattroff(janela, COLOR_PAIR(2));
    for(int i = 1; i < q; i++) {
        mvwprintw(janela, y+1+i, 2, "%s", opcoes[i]);
    }

    // Lendo entrada
    int sely = y+1, sel = 0, entr;
    while(true) {
        entr = wgetch(janela);

        switch(entr) {
            case KEY_UP:
                if(sel) {
                    // Tira seleção
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);

                    // Coloca no item acima
                    sely--;
                    sel--;
                    wattron(janela, COLOR_PAIR(2));
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                    wattroff(janela, COLOR_PAIR(2));
                }
                break;
            case KEY_DOWN:
                if(sel<q-1) {
                    // Tira seleção
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);

                    // Coloca no item acima
                    sely++;
                    sel++;
                    wattron(janela, COLOR_PAIR(2));
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                    wattroff(janela, COLOR_PAIR(2));
                }
                break;
            case 10:
                wattron(janela, COLOR_PAIR(1));
                mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                wattroff(janela, COLOR_PAIR(1));

                refresh();
                wrefresh(janela);

                return sel;
            case 27:
                return -1;
        }
        
        refresh();
        wrefresh(janela);
    }

}

int scanOpcoesExclusao(WINDOW* janela, int y) {
    /*
    Função para receber entrada, essa em específica é de acordo com a opção marcasa

    janela -> Referênte a janela que estará isso
    opcoes -> Opções
    q -> Quantidade de opções

    Se retorna -1, é que saiu do campo de digitação com 'esc'
    */

    char titulo[] = "Deseja excluir mesmo?";
    char opcoes[2][50] = {"Não", "Sim"};
    int q = 2;

    // Título
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "%s", titulo);
    wattroff(janela, A_BOLD);

    // Opções
    wattron(janela, COLOR_PAIR(2));
    mvwprintw(janela, y+1, 2, "%s", opcoes[0]);
    wattroff(janela, COLOR_PAIR(2));
    for(int i = 1; i < q; i++) {
        mvwprintw(janela, y+1+i, 2, "%s", opcoes[i]);
    }

    // Lendo entrada
    int sely = y+1, sel = 0, entr;
    while(true) {
        entr = wgetch(janela);

        switch(entr) {
            case KEY_UP:
                if(sel) {
                    // Tira seleção
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);

                    // Coloca no item acima
                    sely--;
                    sel--;
                    wattron(janela, COLOR_PAIR(2));
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                    wattroff(janela, COLOR_PAIR(2));
                }
                break;
            case KEY_DOWN:
                if(sel<q-1) {
                    // Tira seleção
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);

                    // Coloca no item acima
                    sely++;
                    sel++;
                    wattron(janela, COLOR_PAIR(3));
                    mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                    wattroff(janela, COLOR_PAIR(3));
                }
                break;
            case 10:
                wattron(janela, COLOR_PAIR(1));
                mvwprintw(janela, sely, 2, "%s", opcoes[sel]);
                wattroff(janela, COLOR_PAIR(1));

                refresh();
                wrefresh(janela);

                return sel;
            case 27:
                return -1;
        }
        
        refresh();
        wrefresh(janela);
    }

}

int scanNum(WINDOW* janela, int y, char titulo[]) {
    // Se retorna -1, é que saiu do campo de digitação com 'esc'

    // Obtendo largura
    int w = getmaxx(janela);

    // Visibilidade do cursor ativado
    curs_set(1);

    // Título do campo de texto
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "%s", titulo);
    wattroff(janela, A_BOLD);
    // Entrada
    int num_entr=0, entr=0;
    wattron(janela, A_UNDERLINE);
    mvwhline(janela, y+1, 2, ' ', w-4);
    wmove(janela, y+1, 2);
    while(entr != 10 || getcurx(janela) == 2) {
        entr = wgetch(janela);

        if(entr > 47 && entr < 58) {
            double num_test = (double)num_entr*10 + (double)(entr-48);
            if(num_test <= INT_MAX) {
                waddch(janela, entr);
                num_entr = num_entr*10 + (entr-48);
            }
        } else {
            if(entr == KEY_BACKSPACE && getcurx(janela) > 2) {
                mvwaddch(janela, y+1, getcurx(janela)-1, ' ');
                wmove(janela, y+1, getcurx(janela)-1);
                num_entr = num_entr/10;
            }
            if(entr == 27) { // Tecla ESC
                // Visibilidade do cursor desativado
                wattroff(janela, A_UNDERLINE);
                curs_set(0);
                return -1;
            }
        }

        refresh();
        wrefresh(janela);
    }
    wattroff(janela, A_UNDERLINE);

    // Visibilidade do cursor desativado
    curs_set(0);

    return num_entr;
}

double scanFloat(WINDOW* janela, int y, char titulo[]) {
    // Se retorna -1, é que saiu do campo de digitação com 'esc'

    // Obtendo largura
    int w = getmaxx(janela);

    // Visibilidade do cursor ativado
    curs_set(1);

    // Título do campo de texto
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "%s", titulo);
    wattroff(janela, A_BOLD);
    // Entrada
    int num_entr=0, entr=0;
    wattron(janela, A_UNDERLINE);
    mvwhline(janela, y+1, 2, ' ', w-4);
    mvwprintw(janela, y+1, 2, "%.2lf", num_entr/100.0);
    while(entr != 10 || getcurx(janela) == 2) {
        entr = wgetch(janela);

        if(entr > 47 && entr < 58) {
            double num_test = (double)num_entr*10 + (double)(entr-48);
            if(num_test <= INT_MAX) {
                num_entr = num_entr*10 + (entr-48);
                mvwhline(janela, y+1, 2, ' ', w-4);
                mvwprintw(janela, y+1, 2, "%.2lf", num_entr/100.0);
            }
        } else {
            if(entr == KEY_BACKSPACE && getcurx(janela) > 2) {
                num_entr = num_entr/10;
                mvwhline(janela, y+1, 2, ' ', w-4);
                mvwprintw(janela, y+1, 2, "%.2lf", num_entr/100.0);
            }
            if(entr == 27) { // Tecla ESC
                // Visibilidade do cursor desativado
                wattroff(janela, A_UNDERLINE);
                curs_set(0);
                return -1;
            }
        }

        refresh();
        wrefresh(janela);
    }
    wattroff(janela, A_UNDERLINE);

    // Visibilidade do cursor desativado
    curs_set(0);

    return num_entr/100.0;
}

int scanText(WINDOW* janela, int y, char titulo[], int max, char* dest) {
    // Se retorna -1, é que saiu do campo de digitação com 'esc'

    // Obtendo largura
    int w = getmaxx(janela);

    // Visibilidade do cursor ativado
    curs_set(1);

    // Título do campo de texto
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "%s", titulo);
    wattroff(janela, A_BOLD);
    // Entrada
    char str_entr[max+1]; str_entr[0] = '\0';
    int str_i = 0;
    int entr=0;
    wattron(janela, A_UNDERLINE);
    mvwhline(janela, y+1, 2, ' ', w-4);
    wmove(janela, y+1, 2);
    while(entr != 10 || getcurx(janela) == 2) {
        entr = wgetch(janela);

        if(entr == KEY_BACKSPACE) { // Apagando
            if(getcurx(janela) > 2) {
                mvwaddch(janela, y+1, getcurx(janela)-1, ' ');
                wmove(janela, y+1, getcurx(janela)-1);
            
                str_i--;
                str_entr[str_i] = '\0';
            }
        } else {
            if(entr == 27) { // Tecla ESC
                // Visibilidade do cursor desativado
                wattroff(janela, A_UNDERLINE);
                curs_set(0);
                return -1;
            } else {
                if(str_i < max-1 && entr != '\t' && entr != 10 && entr != ';') { // Escrevendo
                    waddch(janela, entr);
                    str_entr[str_i] = (char)entr;
                    str_i++;
                    str_entr[str_i] = '\0';
                }
            }
        }

        refresh();
        wrefresh(janela);
    }
    wattroff(janela, A_UNDERLINE);

    strcpy(dest, str_entr);
    // Visibilidade do cursor desativado
    curs_set(0);
    return 1;
}

int scanTextCPF(WINDOW* janela, int y, char* dest) {
    // Se retorna -1, é que saiu do campo de digitação com 'esc'

    // Obtendo largura
    int w = getmaxx(janela);

    // Visibilidade do cursor ativado
    curs_set(1);

    // Título do campo de texto
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "CPF");
    wattroff(janela, A_BOLD);
    // Entrada
    char str_entr[15]; str_entr[14] = '\0';
    int str_i = 0;
    int entr = 0;
    wattron(janela, A_UNDERLINE);
    mvwhline(janela, y+1, 2, ' ', w-4);
    mvwprintw(janela, y+1, 2, "   .   .   -  ");
    wmove(janela, y+1, 2);
    while(entr != 10 || getcurx(janela) != 16) {
        printPadrao(janela, y+1, 2, "   .   .   -  ");
        entr = wgetch(janela);

        if(entr > 47 && entr < 58) {
            if(str_i < 14) {
                waddch(janela, entr);
                str_entr[str_i] = (char)entr;
                str_i++;
                if(str_i == 3 || str_i == 7 || str_i == 11) {
                    if(str_i != 11) {
                        str_entr[str_i] = '.';
                    } else {
                        str_entr[str_i] = '-';
                    }
                    str_i++;
                    wmove(janela, y+1, getcurx(janela)+1);
                }
            }
        } else {
            if(entr == KEY_BACKSPACE && getcurx(janela) > 2) {
                mvwaddch(janela, y+1, getcurx(janela)-1, ' ');
                wmove(janela, y+1, getcurx(janela)-1);
                str_i--;

                if(str_i-1 == 3 || str_i-1 == 7 || str_i-1 == 11) {
                    wmove(janela, y+1, getcurx(janela)-1);
                    str_i--;
                }
            }
            if(entr == 27) { // Tecla ESC
                // Visibilidade do cursor desativado
                wattroff(janela, A_UNDERLINE);
                curs_set(0);
                return -1;
            }
        }

        refresh();
        wrefresh(janela);
    }
    wattroff(janela, A_UNDERLINE);

    strcpy(dest, str_entr);
    curs_set(0);
    return 1;
}

int scanTextCNPJ(WINDOW* janela, int y, char* dest) {
    // Se retorna -1, é que saiu do campo de digitação com 'esc'

    // Obtendo largura
    int w = getmaxx(janela);

    // Visibilidade do cursor ativado
    curs_set(1);

    // Título do campo de texto
    wattron(janela, A_BOLD);
    mvwprintw(janela, y, 2, "CNPJ");
    wattroff(janela, A_BOLD);
    // Entrada
    char str_entr[19]; str_entr[18] = '\0';
    int str_i = 0;
    int entr = 0;
    wattron(janela, A_UNDERLINE);
    mvwhline(janela, y+1, 2, ' ', w-4);
    mvwprintw(janela, y+1, 2, "  .   .   /    -  ");
    wmove(janela, y+1, 2);
    while(entr != 10 || getcurx(janela) != 20) {
        printPadrao(janela, y+1, 2, "  .   .   /    -  ");
        entr = wgetch(janela);

        if(entr > 47 && entr < 58) {
            if(str_i < 18) {
                waddch(janela, entr);
                str_entr[str_i] = (char)entr;
                str_i++;
                if(str_i == 2 || str_i == 6 || str_i == 10 || str_i == 15) {
                    if(str_i < 10) {
                        str_entr[str_i] = '.';
                    } else {
                        if(str_i == 10) {
                            str_entr[str_i] = '/';
                        } else {
                            str_entr[str_i] = '-';
                        }
                    }
                    str_i++;
                    wmove(janela, y+1, getcurx(janela)+1);
                }
            }
        } else {
            if(entr == KEY_BACKSPACE && getcurx(janela) > 2) {
                mvwaddch(janela, y+1, getcurx(janela)-1, ' ');
                wmove(janela, y+1, getcurx(janela)-1);
                str_i--;

                if(str_i-1 == 2 || str_i-1 == 6 || str_i-1 == 10 || str_i-1 == 15) {
                    wmove(janela, y+1, getcurx(janela)-1);
                    str_i--;
                }
            }
            if(entr == 27) { // Tecla ESC
                // Visibilidade do cursor desativado
                wattroff(janela, A_UNDERLINE);
                curs_set(0);
                return -1;
            }
        }

        refresh();
        wrefresh(janela);
    }
    wattroff(janela, A_UNDERLINE);

    strcpy(dest, str_entr);
    curs_set(0);
    return 1;
}

// Função de imprimir linhas da lista
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

void printLinhaItemPedido(WINDOW* janela, int y, int x, int c, ItemPedido ip) {
    /*
    Função para fazer a linha da tabela, essa linha terá as informações do item pedido

    Os argumentos são:
    janela -> Referênte a janela que a linha estará
    y -> Localização do inicio em y
    x -> Localização do inicio em x
    c -> Comprimento, começa em (x,y) e termina em (x+c-1,y)
    ip -> Item pedido individual, o conteúdo da linha
    */
    char temp[250];

    mvwhline(janela, y, x, ' ', c);
    sprintf(temp, "%d", ip.pedidoId); printTextoLim(janela, y, x+((c)/4)*0, temp, (x+((c)/4)*1)-(x+((c)/4)*0)-1);
    sprintf(temp, "%d", ip.produtoId); printTextoLim(janela, y, x+((c)/4)*1, temp, (x+((c)/4)*2)-(x+((c)/4)*1)-1);
    sprintf(temp, "%d", ip.quantidade); printTextoLim(janela, y, x+((c)/4)*2, temp, (x+((c)/4)*3)-(x+((c)/4)*2)-1);
    sprintf(temp, "%.2lf reais", ip.subtotal); printTextoLim(janela, y, x+((c)/4)*3, temp, (x+((c)/4)*4)-(x+((c)/4)*3)-1);
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
    char temp[250];

    mvwhline(janela, y, x, ' ', c);
    sprintf(temp, "%d", pd.id); printTextoLim(janela, y, x+((c)/4)*0, temp, (x+((c)/4)*1)-(x+((c)/4)*0)-1);
    sprintf(temp, "%d", pd.clienteId); printTextoLim(janela, y, x+((c)/4)*1, temp, (x+((c)/4)*2)-(x+((c)/4)*1)-1);
    printTextoLim(janela, y, x+((c)/4)*2, pd.data, (x+((c)/4)*3)-(x+((c)/4)*2)-1);
    sprintf(temp, "%.2lf reais", pd.total); printTextoLim(janela, y, x+((c)/4)*3, temp, (x+((c)/4)*4)-(x+((c)/4)*3)-1);
}

void printLinhaProduto(WINDOW* janela, int y, int x, int c, Produto* prd) {
    /*
    Função para fazer a linha da tabela, essa linha terá as informações do pedido

    Os argumentos são:
    janela -> Referênte a janela que a linha estará
    y -> Localização do inicio em y
    x -> Localização do inicio em x
    c -> Comprimento, começa em (x,y) e termina em (x+c-1,y)
    prd -> Produto individual, o conteúdo da linha
    */
    char temp[250];

    mvwhline(janela, y, x, ' ', c);
    sprintf(temp, "%d", (*prd).id); printTextoLim(janela, y, x+((c)/4)*0, temp, (x+((c)/4)*1)-(x+((c)/4)*0)-1);
    printTextoLim(janela, y, x+((c)/4)*1, (*prd).descricao, (x+((c)/4)*2)-(x+((c)/4)*1)-1);
    sprintf(temp, "%.2lf reais", (*prd).preco); printTextoLim(janela, y, x+((c)/4)*2, temp, (x+((c)/4)*3)-(x+((c)/4)*2)-1);
    sprintf(temp, "%d", (*prd).estoque); printTextoLim(janela, y, x+((c)/4)*3, temp, (x+((c)/4)*4)-(x+((c)/4)*3)-1);
}

void printLinhaCliente(WINDOW* janela, int y, int x, int c, cliente cl) {
    char temp[250];

    mvwhline(janela, y, x, ' ', c);
    sprintf(temp, "%d", cl.id); printTextoLim(janela, y, x+((c)/4)*0, temp, (x+((c)/4)*1)-(x+((c)/4)*0)-1);
    printTextoLim(janela, y, x+((c)/4)*1, cl.nome, (x+((c)/4)*2)-(x+((c)/4)*1)-1);
    printTextoLim(janela, y, x+((c)/4)*2, cl.telefone, (x+((c)/4)*3)-(x+((c)/4)*2)-1);
    printTextoLim(janela, y, x+((c)/4)*3, cl.endereco, (x+((c)/4)*4)-(x+((c)/4)*3)-1);
}

// Função de imprimir lista
void mostrarListaItensPedido(WINDOW* janela, int y, pedido* p) {
    int hj, wj; getmaxyx(janela, hj, wj); // Dimensões da janela
    char tags[4][50] = {"ID do pedido", "ID produto", "Quantidade", "Subtotal"}; // Tags da lista

    for(int i = y; i < hj-2; i++) {
        mvwhline(janela, i, 2, ' ', wj-4);
    }
    if((*p).quant_itens == 0) {
        mvwprintw(janela, y+2, wj/2-11, "Nenhum item adicionado");
    }

    // Linha das tags
    printPLinhaUni(janela, y, 2, wj-4, tags, 4);
    // e as demais linhas, que já são os itens em si
    for(int i = 0; i < (*p).quant_itens && (y+1)+i < hj-3; i++) {
        printLinhaItemPedido(janela, y+1+i, 2, wj-4, *((*p).itens+i));
    }
    if((*p).quant_itens > (hj-3)-(y+1)) {
        mvwprintw(janela, hj-3, 2, "...");
    }
    // ..., a primeira linha está selecionada
    if((*p).quant_itens != 0) {
        wattron(janela, COLOR_PAIR(2));
        printLinhaItemPedido(janela, y+1, 2, wj-4, *((*p).itens));
        wattroff(janela, COLOR_PAIR(2));
    }
    // Atualizando a janela
    refresh();
    wrefresh(janela);

    // Deslocando
    int ini=y+1, sel=0, sely=ini, entr; // Y do inicio da lista; Seleção; Y da seleção; Entrada do teclado
    while(true) {
        entr = wgetch(janela);

        if((*p).quant_itens == 0) {
            if(entr == 27) {
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            } else {
                continue;
            }
        }

        switch(entr) {
            case KEY_UP:
                if(!(sely == y+1 && sel > 0)) { // Ainda não é o primeiro aparecendo visualmente
                    if(sel) { // Não sendo 0, ainda dá para subir
                        // A linha selecionada perde a seleção
                        printLinhaItemPedido(janela, sely, 2, wj-4, *((*p).itens+sel));
                        
                        // Muda a seleção
                        sel--;
                        sely--;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaItemPedido(janela, sely, 2, wj-4, *((*p).itens+sel));
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
                    printLinhaItemPedido(janela, y+1, 2, wj-4, *((*p).itens+sel));
                    wattroff(janela, COLOR_PAIR(2));
                    for(int i = 1; i < (hj-3)-(y+1); i++) {
                        printLinhaItemPedido(janela, y+1+i, 2, wj-4, *((*p).itens+sel+i));
                    }

                    // Ao scrollar, os últimos conteúdos sumiriam, voltar os 3 pontinhos
                    mvwprintw(janela, hj-3, 2, "...");

                    // Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case KEY_DOWN:
                if(!(sely == hj-4 && sel < (*p).quant_itens-1)) { // Ainda não é o último aparecendo visualmente
                    if(sel < (*p).quant_itens-1) { // Ainda menor que o último índice, ainda dá para descer
                        // A linha selecionada perde a seleção
                        printLinhaItemPedido(janela, sely, 2, wj-4, *((*p).itens+sel));

                        // Muda a seleção
                        sel++;
                        sely++;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaItemPedido(janela, sely, 2, wj-4, *((*p).itens+sel));
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
                        printLinhaItemPedido(janela, y+1+i, 2, wj-4, *((*p).itens+sel-((hj-3)-(y+1)-1)+i));
                    }
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaItemPedido(janela, hj-4, 2, wj-4, *((*p).itens+sel));
                    wattroff(janela, COLOR_PAIR(2));

                    // Se chegar ao final, tirar os 3 pontinhos
                    if(sel == (*p).quant_itens-1) {
                        mvwprintw(janela, hj-3, 2, "   ");
                    }

                    //Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case 27:
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
        }
    }
}

void mostrarListaPedidos(WINDOW* janela, int y, ListaPedido* lp, ListaProduto* lpr, int** janela_backup) {
    int hj, wj; getmaxyx(janela, hj, wj); // Dimensões da janela
    char tags[4][50] = {"ID do pedido", "ID do cliente", "Data", "Total"}; // Tags da lista

    for(int i = y; i < hj-2; i++) {
        mvwhline(janela, i, 2, ' ', wj-4);
    }
    if((*lp).quant == 0) {
        mvwprintw(janela, y+2, wj/2-11, "Nenhum item adicionado");
    }

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
    if((*lp).quant != 0) {
        wattron(janela, COLOR_PAIR(2));
        printLinhaPedido(janela, y+1, 2, wj-4, *((*lp).pedidos));
        wattroff(janela, COLOR_PAIR(2));
    }
    // Atualizando a janela
    refresh();
    wrefresh(janela);

    // Deslocando
    int ini=y+1, sel=0, sely=ini, entr; // Y do inicio da lista; Seleção; Y da seleção; Entrada do teclado
    while(true) {
        entr = wgetch(janela);

        if((*lp).quant == 0) {
            if(entr == 27) {
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            } else {
                continue;
            }
        }

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
            case 27:
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            case 10:
                ligarBlur(janela, janela_backup);

                mostrarSubSubMenuGerenciarItPedido(lpr, ((*lp).pedidos+sel));

                desligarBlur(janela, janela_backup);

                // Reescrevendo por conta do bug do UTF-8
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
                if((*lp).quant != 0) {
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaPedido(janela, y+1, 2, wj-4, *((*lp).pedidos));
                    wattroff(janela, COLOR_PAIR(2));
                }
                sel=0; sely=ini;

                refresh();
                wrefresh(janela);

                break;
        }
    }
}

void mostrarListaProdutos(WINDOW* janela, int y, ListaProduto* lpr) {
    int hj, wj; getmaxyx(janela, hj, wj); // Dimensões da janela
    char tags[4][50] = {"ID", "Descrição", "Preço", "Estoque"}; // Tags da lista

    for(int i = y; i < hj-2; i++) {
        mvwhline(janela, i, 2, ' ', wj-4);
    }
    if((*lpr).quant == 0) {
        mvwprintw(janela, y+2, wj/2-11, "Nenhum item adicionado");
    }

    // Linha das tags
    printPLinhaUni(janela, y, 2, wj-4, tags, 4);
    // e as demais linhas, que já são os itens em si
    for(int i = 0; i < (*lpr).quant && (y+1)+i < hj-3; i++) {
        printLinhaProduto(janela, y+1+i, 2, wj-4, ((*lpr).produtos+i));
    }
    if((*lpr).quant > (hj-3)-(y+1)) {
        mvwprintw(janela, hj-3, 2, "...");
    }
    // ..., a primeira linha está selecionada
    if((*lpr).quant != 0) {
        wattron(janela, COLOR_PAIR(2));
        printLinhaProduto(janela, y+1, 2, wj-4, ((*lpr).produtos));
        wattroff(janela, COLOR_PAIR(2));
    }
    // Atualizando a janela
    refresh();
    wrefresh(janela);

    // Deslocando
    int ini=y+1, sel=0, sely=ini, entr; // Y do inicio da lista; Seleção; Y da seleção; Entrada do teclado
    while(true) {
        entr = wgetch(janela);

        if((*lpr).quant == 0) {
            if(entr == 27) {
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            } else {
                continue;
            }
        }

        switch(entr) {
            case KEY_UP:
                if(!(sely == y+1 && sel > 0)) { // Ainda não é o primeiro aparecendo visualmente
                    if(sel) { // Não sendo 0, ainda dá para subir
                        // A linha selecionada perde a seleção
                        printLinhaProduto(janela, sely, 2, wj-4, ((*lpr).produtos+sel));
                        
                        // Muda a seleção
                        sel--;
                        sely--;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaProduto(janela, sely, 2, wj-4, ((*lpr).produtos+sel));
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
                    printLinhaProduto(janela, y+1, 2, wj-4, ((*lpr).produtos+sel));
                    wattroff(janela, COLOR_PAIR(2));
                    for(int i = 1; i < (hj-3)-(y+1); i++) {
                        printLinhaProduto(janela, y+1+i, 2, wj-4, ((*lpr).produtos+sel+i));
                    }

                    // Ao scrollar, os últimos conteúdos sumiriam, voltar os 3 pontinhos
                    mvwprintw(janela, hj-3, 2, "...");

                    // Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case KEY_DOWN:
                if(!(sely == hj-4 && sel < (*lpr).quant-1)) { // Ainda não é o último aparecendo visualmente
                    if(sel < (*lpr).quant-1) { // Ainda menor que o último índice, ainda dá para descer
                        // A linha selecionada perde a seleção
                        printLinhaProduto(janela, sely, 2, wj-4, ((*lpr).produtos+sel));

                        // Muda a seleção
                        sel++;
                        sely++;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaProduto(janela, sely, 2, wj-4, ((*lpr).produtos+sel));
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
                        printLinhaProduto(janela, y+1+i, 2, wj-4, ((*lpr).produtos+sel-((hj-3)-(y+1)-1)+i));
                    }
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaProduto(janela, hj-4, 2, wj-4, ((*lpr).produtos+sel));
                    wattroff(janela, COLOR_PAIR(2));

                    // Se chegar ao final, tirar os 3 pontinhos
                    if(sel == (*lpr).quant-1) {
                        mvwprintw(janela, hj-3, 2, "   ");
                    }

                    //Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case 27:
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
        }
    }
}

void mostrarListaClientes(WINDOW* janela, int y, ListaCliente* clt, int** janela_backup, ListaCpf* cpfs, ListaCnpj* cnpjs) {
    int hj, wj; getmaxyx(janela, hj, wj); // Dimensões da janela
    char tags[4][50] = {"ID", "Nome", "Telefone", "Endereço"}; // Tags da lista

    for(int i = y; i < hj-2; i++) {
        mvwhline(janela, i, 2, ' ', wj-4);
    }
    if((*clt).quant == 0) {
        mvwprintw(janela, y+2, wj/2-11, "Nenhum item adicionado");
    }

    // Linha das tags
    printPLinhaUni(janela, y, 2, wj-4, tags, 4);
    // e as demais linhas, que já são os itens em si
    for(int i = 0; i < (*clt).quant && (y+1)+i < hj-3; i++) {
        printLinhaCliente(janela, y+1+i, 2, wj-4, *((*clt).clientes+i));
    }
    if((*clt).quant > (hj-3)-(y+1)) {
        mvwprintw(janela, hj-3, 2, "...");
    }
    // ..., a primeira linha está selecionada
    if((*clt).quant != 0) {
        wattron(janela, COLOR_PAIR(2));
        printLinhaCliente(janela, y+1, 2, wj-4, *((*clt).clientes));
        wattroff(janela, COLOR_PAIR(2));
    }
    // Atualizando a janela
    refresh();
    wrefresh(janela);

    // Deslocando
    int ini=y+1, sel=0, sely=ini, entr; // Y do inicio da lista; Seleção; Y da seleção; Entrada do teclado
    while(true) {
        entr = wgetch(janela);

        if((*clt).quant == 0) {
            if(entr == 27) {
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            } else {
                continue;
            }
        }

        switch(entr) {
            case KEY_UP:
                if(!(sely == y+1 && sel > 0)) { // Ainda não é o primeiro aparecendo visualmente
                    if(sel) { // Não sendo 0, ainda dá para subir
                        // A linha selecionada perde a seleção
                        printLinhaCliente(janela, sely, 2, wj-4, *((*clt).clientes+sel));
                        
                        // Muda a seleção
                        sel--;
                        sely--;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaCliente(janela, sely, 2, wj-4, *((*clt).clientes+sel));
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
                    printLinhaCliente(janela, y+1, 2, wj-4, *((*clt).clientes+sel));
                    wattroff(janela, COLOR_PAIR(2));
                    for(int i = 1; i < (hj-3)-(y+1); i++) {
                        printLinhaCliente(janela, y+1+i, 2, wj-4, *((*clt).clientes+sel+i));
                    }

                    // Ao scrollar, os últimos conteúdos sumiriam, voltar os 3 pontinhos
                    mvwprintw(janela, hj-3, 2, "...");

                    // Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case KEY_DOWN:
                if(!(sely == hj-4 && sel < (*clt).quant-1)) { // Ainda não é o último aparecendo visualmente
                    if(sel < (*clt).quant-1) { // Ainda menor que o último índice, ainda dá para descer
                        // A linha selecionada perde a seleção
                        printLinhaCliente(janela, sely, 2, wj-4, *((*clt).clientes+sel));

                        // Muda a seleção
                        sel++;
                        sely++;

                        // A linha não selecionada virá a selecionada
                        wattron(janela, COLOR_PAIR(2));
                        printLinhaCliente(janela, sely, 2, wj-4, *((*clt).clientes+sel));
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
                        printLinhaCliente(janela, y+1+i, 2, wj-4, *((*clt).clientes+sel-((hj-3)-(y+1)-1)+i));
                    }
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaCliente(janela, hj-4, 2, wj-4, *((*clt).clientes+sel));
                    wattroff(janela, COLOR_PAIR(2));

                    // Se chegar ao final, tirar os 3 pontinhos
                    if(sel == (*clt).quant-1) {
                        mvwprintw(janela, hj-3, 2, "   ");
                    }

                    //Atualiza
                    refresh();
                    wrefresh(janela);
                }
                break;
            case 27:
                for(int i = y; i < hj-2; i++) {
                    mvwhline(janela, i, 2, ' ', wj-4);
                }

                refresh();
                wrefresh(janela);
                return;
                break;
            case 10:
                ligarBlur(janela, janela_backup);

                mostrarCliente(((*clt).clientes+sel), cpfs, cnpjs);

                desligarBlur(janela, janela_backup);

                // Reescrevendo por conta do bug do UTF-8
                // Linha das tags
                printPLinhaUni(janela, y, 2, wj-4, tags, 4);
                // e as demais linhas, que já são os itens em si
                for(int i = 0; i < (*clt).quant && (y+1)+i < hj-3; i++) {
                    printLinhaCliente(janela, y+1+i, 2, wj-4, *((*clt).clientes+i));
                }
                if((*clt).quant > (hj-3)-(y+1)) {
                    mvwprintw(janela, hj-3, 2, "...");
                }
                // ..., a primeira linha está selecionada
                if((*clt).quant != 0) {
                    wattron(janela, COLOR_PAIR(2));
                    printLinhaCliente(janela, y+1, 2, wj-4, *((*clt).clientes));
                    wattroff(janela, COLOR_PAIR(2));
                }
                sel=0; sely=ini;

                refresh();
                wrefresh(janela);

                break;
        }
    }
}

// Estéticas
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

void ligarBlur(WINDOW* janela, int** janela_backup) {
    int y, x;
    getmaxyx(janela, y, x);

    for(int i = 0; i < y; i++) {
        for(int j = 0; j < x; j++) {
            *(*(janela_backup+i)+j) = mvwinch(janela, i, j);
            if(mvwinch(janela, i, j) != ' ') {
                mvwaddch(janela, i, j, ':');
            }
        }
    }

    refresh();
    wrefresh(janela);
}

void desligarBlur(WINDOW* janela, int** janela_backup) {
    int y, x;
    getmaxyx(janela, y, x);

    for(int i = 0; i < y; i++) {
        for(int j = 0; j < x; j++) {
            mvwaddch(janela, i, j, *(*(janela_backup+i)+j));
        }
    }

    refresh();
    wrefresh(janela);
}

// Opções dos menus
int mostrarOpcoesMenuUni(WINDOW* janela, char opcoes[][50], int q, int pre_s) {
    /*
    Função para mostrar opções e retornar seleção do menu universalmente

    ATENÇÃO: A função não é compátivel com scroll, é esperado que não ultrapasse o tamanho da janela, por ser poucas opções nos casos de uso

    Os argumentos são:
    janela -> Referênte a janela
    opcoes -> Opções existentes, a primeira é resaltada, por convenção interna, a primeira se trata de uma lista
    q -> Quantidade de opções
    pre_s -> Pre_seleção

    ATENÇÃO: A lista provavelmente nunca atingirá 114 itens, mas se ocorrer, 113 (último) é igual ao 'q'no ascii e ocorrerá a finalização do menu
    */    

    // Opções impressas
    if(!pre_s) { // Se for zero, marcar como selecionado o primeiro
        wattron(janela, COLOR_PAIR(2));
        mvwprintw(janela, 2, 2, "%s", opcoes[0]);
        wattroff(janela, COLOR_PAIR(2));
    } else {
        mvwprintw(janela, 2, 2, "%s", opcoes[0]);
    }
    for(int i = 1; i < q; i++) { // Imprimindo as opções
        mvwprintw(janela, 3+i, 2, "%s", opcoes[i]);
    }
    if(pre_s) { // Se não for zero, marcar como selecionado, o pre_s
        wattron(janela, COLOR_PAIR(2));
        mvwprintw(janela, 3+pre_s, 2, "%s", opcoes[pre_s]);
        wattroff(janela, COLOR_PAIR(2));
    }

    // Seleção
    int sel=pre_s, entr;
    while(true) {
        entr = wgetch(janela);

        switch(entr) {
            case KEY_UP:
                if(sel) {
                    if(sel == 1) { // Se for a segunda seleção
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);

                        wattron(janela, COLOR_PAIR(2));
                        mvwprintw(janela, 2, 2, "%s", opcoes[0]);
                        wattroff(janela, COLOR_PAIR(2));

                        sel--;
                    } else {
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);

                        sel--;

                        wattron(janela, COLOR_PAIR(2));
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);
                        wattroff(janela, COLOR_PAIR(2));
                    }
                }
                break;
            case KEY_DOWN:
                if(sel < q-1) {
                    if(!sel) {
                        mvwprintw(janela, 2, 2, "%s", opcoes[0]);

                        sel++;

                        wattron(janela, COLOR_PAIR(2));
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);
                        wattroff(janela, COLOR_PAIR(2));
                    } else {
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);

                        sel++;

                        wattron(janela, COLOR_PAIR(2));
                        mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);
                        wattroff(janela, COLOR_PAIR(2));
                    }
                }
                break;
            case 10:
                if(sel == 0) {
                    mvwprintw(janela, 2, 2, "%s", opcoes[0]);
                } else {
                    mvwprintw(janela, 3+sel, 2, "%s", opcoes[sel]);
                }
                return sel;
                break;
            case 27:
                return 27;
                break;
        }

        refresh();
        wrefresh(janela);
    }
} 

// =========== Outras janelas ===========

void mostrarAlerta(bool critico, char texto[]) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/3-2, wj = w-w/2-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50];
    if(!critico) {
        strcpy(titulo, "Atenção");
    } else {
        strcpy(titulo, "Erro");
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    wattron(janela, A_BOLD);
    if(critico) {
        wattron(janela, COLOR_PAIR(4));
    } else {
        wattron(janela, COLOR_PAIR(5));
    }
    box(janela, 0, 0); // Contorno padrão

    // Adicionando titulo
    printTituloJanela(janela, titulo, (critico)? 4 : 7); // Título
    printTexto(janela, 2, 2, texto);

    refresh();
    wrefresh(janela);

    // Finalizando
    wgetch(janela);
    wattroff(janela, A_BOLD);
    if(critico) {
        wattroff(janela, COLOR_PAIR(4));
    } else {
        wattroff(janela, COLOR_PAIR(5));
    }
}

void mostrarSubSubMenuRemoverItem(pedido* p) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Remoção do item"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};
    bool temp;
    int entr = 0;

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 15); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // ID do produto
    int id_produto = scanNum(janela, 2, "ID do produto");
    if(id_produto == -1) {
        delwin(janela);
        refresh();
        return;
    }

    int tipo = scanOpcoesExclusao(janela, 5);
    if(tipo == -1) {
        delwin(janela);
        refresh();
        return;
    }

    if(tipo) {
        temp = removerItemPedido(p, id_produto);
        if(!temp) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 8, 2, "Item com o produto correspondente não encontrado");

            while(entr != 10 && entr != 27) {
                entr = wgetch(janela);
            }

            delwin(janela);
            refresh();
            return;
        }
    }

    refresh();
    wrefresh(janela);

    // Finalizando
    delwin(janela);
    refresh();
}

void mostrarSubSubMenuCadastrarItem(ListaProduto* lpr, pedido* p) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-6, wj = w-w/10-6; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Cadastro do item"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"}; // Opções da janela
    int temp;

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 19); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Entradas do usuário
    int id_produto;
    int quant;

    // Id produto
    bool analise_id2=false;
    while(!analise_id2) {
        id_produto = scanNum(janela, 2, "ID do produto");

        if(id_produto == -1) {
            // Finalizando
            delwin(janela);
            refresh();
            return;
        }

        analise_id2 = analisarProduto(lpr, id_produto);
        if(!analise_id2) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }
  
    // Quant
    quant = scanNum(janela, 5, "Quantidade");
    if(quant == -1) {
        delwin(janela);
        refresh();
        return;
    }

    ItemPedido ip;
    ip.pedidoId = (*p).id;
    ip.produtoId = id_produto;
    ip.quantidade = quant;
    ip.subtotal = (*consultarProduto(lpr, id_produto)).preco * quant;
    temp = cadastrarItemPedido(p, ip);
    if(!temp) {
        ligarBlur(janela, janela_backup);

        mostrarAlerta(true, "Não foi possível adicionar o item do pedido, verifique o uso da memória RAM ou reporte para um dos desenvolvedores");

        desligarBlur(janela, janela_backup);
    }

    // Finalizando
    delwin(janela);
    refresh();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubSubMenuGerenciarItPedido(ListaProduto* lpr, pedido* pd) { //
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-4, wj = w-w/10-4; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Cadastro de itens pedido"; // Título da janela
    char opcoes[2][50] = {"Esc", "Voltar"};
    char opcoes_menu[4][50] = {"> Lista de itens pedidos", "Cadastrar", "Remover"};
    
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 24); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções da janela

    // Seleção do menu
    int entr=0;
    while(true) {
        entr = mostrarOpcoesMenuUni(janela, opcoes_menu, 3, entr);

        switch(entr) {
            case 0:
                mvwprintw(janela, 2, 2, "v Lista de itens pedidos");
                mostrarListaItensPedido(janela, 3, pd);
                break;
            case 1:
                ligarBlur(janela, janela_backup);

                mostrarSubSubMenuCadastrarItem(lpr, pd);

                desligarBlur(janela, janela_backup);
                break;
            case 2:
                ligarBlur(janela, janela_backup);

                mostrarSubSubMenuRemoverItem(pd);

                desligarBlur(janela, janela_backup);
                break;
            case 27:
                delwin(janela);
                refresh();
                return;
        }

        refresh();
        wrefresh(janela);
    }

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubSubMenuMostrarItPedido(pedido* pd) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-4, wj = w-w/10-4; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Lista de itens pedidos"; // Título da janela
    char opcoes[2][50] = {"Esc", "Voltar"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 22); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções da janela

    mostrarListaItensPedido(janela, 2, pd);

    delwin(janela);
    refresh();
}

void mostrarCliente(cliente* cl, ListaCpf* cpfs, ListaCnpj* cnpjs) { //
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-4, wj = w-w/10-4; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Visualização de cliente"; // Título da janela
    char opcoes[2][50] = {"Esc", "Voltar"};
    char temp[250];

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 23); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções da janela

    printTituloBasico(janela, 2, 2, "ID: ");
    sprintf(temp, "%d", (*cl).id); printTextoLim(janela, 3, 2, temp, wj-4);
    printTituloBasico(janela, 5, 2, "Nome: ");
    printTextoLim(janela, 6, 2, (*cl).nome, wj-4);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 8, 2, "Tipo: ");
    wattroff(janela, A_BOLD);
    if(!(*cl).tipo) {
        wprintw(janela, "Pessoa física");

        wattron(janela, A_BOLD);
        mvwprintw(janela, 9, 2, "CPF: ");
        wattroff(janela, A_BOLD);
        wprintw(janela, "%s", (*((*cpfs).cpfs+buscar_cpf((*cl).id, cpfs))).cpf);
    } else {
        wprintw(janela, "Pessoa jurídica");

        wattron(janela, A_BOLD);
        mvwprintw(janela, 9, 2, "CNPJ: ");
        wattroff(janela, A_BOLD);
        wprintw(janela, "%s", (*((*cnpjs).cnpjs+buscar_cnpj((*cl).id, cnpjs))).cnpj);
    }

    printTituloBasico(janela, 11, 2, "Telefone: ");
    printTextoLim(janela, 12, 2, (*cl).telefone, wj-4);
    printTituloBasico(janela, 14, 2, "Endereço: ");
    printTextoLim(janela, 15, 2, (*cl).endereco, wj-4);
    refresh();
    wrefresh(janela);

    int entr = 0;
    while(entr != 27) {
        entr = wgetch(janela);
    }

    delwin(janela);
    refresh();
}

// =========== Sub-menus ===========

void mostrarSubMenuCadastrarPedido(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Cadastro de pedido"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};
    int temp;

    // Backup da janela
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 18); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // ID pedido
    int id_pedido;
    bool id_p_analise=false;
    while(!id_p_analise) {
        id_pedido = scanNum(janela, 2, "ID do pedido");

        if(id_pedido == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_p_analise = analisarPedido(lp, id_pedido);
        if(!id_p_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID já existe");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    // ID cliente
    int id_cliente;
    bool id_c_analise=true;
    while(id_c_analise) {
        id_cliente = scanNum(janela, 5, "ID do cliente");

        if(id_cliente == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_c_analise = verificar_id_cliente(lc, id_cliente);
        if(id_c_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 7, 2, "O ID não existe");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 7, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    // Data
    char data[11];
    temp = scanText(janela, 8, "Data", 11, data);
    if(temp == -1) {
        delwin(janela);
        refresh();
        return;
    }

    // Pré montagem do pedido
    pedido pd;
    pd.clienteId = id_cliente;
    pd.id = id_pedido;
    pd.itens = NULL;
    pd.max_itens = 0;
    pd.quant_itens = 0;
    pd.total = 0;
    strcpy(pd.data, data);

    // Itens do pedido
    char opcoesItem[2][50] = {"Adicionar ou remover itens", "Finalizar e cadastrar o pedido"};
    int opcao = 0;
    while(opcao != 1) {
        opcao = scanOpcoes(janela, 11, "Cadastro dos itens do pedido", opcoesItem, 2);

        if(opcao == -1) {
            delwin(janela);
            refresh();
            return;
        }
        if(opcao == 0) {
            // Cadastrar os itens do pedido
            ligarBlur(janela, janela_backup);
            mostrarSubSubMenuGerenciarItPedido(lpr, &pd);
            desligarBlur(janela, janela_backup);
        }
    }

    cadastrarPedido(pd, lp);

    // ... e finalizando
    delwin(janela);
    refresh();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubMenuCadastrarProduto(ListaProduto* lpr) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Cadastro de produto"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};
    int temp;

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 19); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // ID
    int id_produto;
    bool id_p_analise=true;
    while(id_p_analise) {
        id_produto = scanNum(janela, 2, "ID do produto");

        if(id_produto == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_p_analise = analisarProduto(lpr, id_produto);
        if(id_p_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID já existe");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    // Descrição
    char descricao[100];
    temp = scanText(janela, 5, "Descrição", 100, descricao);
    if(temp == -1) {
        delwin(janela);
        refresh();
        return;
    }

    // Preço
    double preco = scanFloat(janela, 8, "Preço");
    if(preco == -1) {
        delwin(janela);
        refresh();
        return;
    }

    // Estoque
    double quant = scanNum(janela, 11, "Estoque");
    if(quant == -1) {
        delwin(janela);
        refresh();
        return;
    }

    // Adicionando
    Produto pd;
    strcpy(pd.descricao, descricao);
    pd.estoque = quant;
    pd.id = id_produto;
    pd.preco = preco;
    inserirProduto(pd, lpr);
    
    refresh();
    wrefresh(janela);

    // ... e finalizando
    delwin(janela);
    refresh();
}

void mostrarSubMenuCadastrarCliente(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Cadastro de cliente"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"}; // Opções da janela
    int temp;

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 19); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Entradas do usuário
    int id_cliente;
    int tipo;
    char cpfnpj[100];
    char endereco[200];
    char nome[100];
    char telefone[20];

    // Id
    bool analise_id=false;
    while(!analise_id) {
        id_cliente = scanNum(janela, 2, "ID do cliente");

        if(id_cliente == -1) {
            // Finalizando
            delwin(janela);
            refresh();
            return;
        }

        analise_id = verificar_id_cliente(clt, id_cliente);
        if(!analise_id) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID já está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    // Tipo
    char ops_perfil[2][50] = {"Pessoa física", "Pessoa juridica"};
    tipo = scanOpcoes(janela, 5, "Escolha um perfil", ops_perfil, 2);
    if(tipo == -1) {
        // Finalizando
        delwin(janela);
        refresh();
        return;
    }

    // CPF/CNPJ
    bool validacao = false;
    while(!validacao) {
        if(!tipo) {
            temp = scanTextCPF(janela, 9, cpfnpj);
        } else {
            temp = scanTextCNPJ(janela, 9, cpfnpj);
        }

        if(temp == -1) {
            // Finalizando
            delwin(janela);
            refresh();
            return;
        }

        if(!tipo) {
            validacao = Validar_CPF(cpfnpj);
        } else {
            validacao = Validar_CNPJ(cpfnpj);
        }
        if(!validacao) {
            if(!tipo) {
                wattron(janela, COLOR_PAIR(4));
                mvwhline(janela, 11, 2, ' ', wj-4);
                mvwprintw(janela, 11, 2, "O CPF está inválido");
                wattroff(janela, COLOR_PAIR(4));
            } else {
                wattron(janela, COLOR_PAIR(4));
                mvwhline(janela, 11, 2, ' ', wj-4);
                mvwprintw(janela, 11, 2, "O CNPJ está inválido");
                wattroff(janela, COLOR_PAIR(4));
            }
        } else {
            if(!tipo) {
                if(!verificar_cpf(lcp, cpfnpj)) {
                    wattron(janela, COLOR_PAIR(4));
                    mvwhline(janela, 11, 2, ' ', wj-4);
                    mvwprintw(janela, 11, 2, "Esse CPF já está cadastrado");
                    wattroff(janela, COLOR_PAIR(4));
                    validacao = !validacao;
                } else {
                    mvwhline(janela, 11, 2, ' ', wj-4);
                }
            } else {
                if(!verificar_cnpj(lcn, cpfnpj)) {
                    wattron(janela, COLOR_PAIR(4));
                    mvwhline(janela, 11, 2, ' ', wj-4);
                    mvwprintw(janela, 11, 2, "Esse CNPJ já está cadastrado");
                    wattroff(janela, COLOR_PAIR(4));
                    validacao = !validacao;
                } else {
                    mvwhline(janela, 11, 2, ' ', wj-4);
                }
            }
        }

        refresh();
        wrefresh(janela);
    }

    // Nome
    temp = scanText(janela, 12, "Nome", 100, nome);
    if(temp == -1) {
        // Finalizando
        delwin(janela);
        refresh();
        return;
    }

    // Endereço
    temp = scanText(janela, 15, "Endereço", 200, endereco);
    if(temp == -1) {
        // Finalizando
        delwin(janela);
        refresh();
        return;
    }

    // Telefone
    temp = scanText(janela, 18, "Telefone", 20, telefone);
    if(temp == -1) {
        // Finalizando
        delwin(janela);
        refresh();
        return;
    }
  
    cliente cl;
    strcpy(cl.endereco, endereco);
    cl.id = id_cliente;
    strcpy(cl.nome, nome);
    strcpy(cl.telefone, telefone);
    if(!tipo) {
        cl.tipo = TIPO_PESSOA_FISICA;

        PessoaFisica ps;
        ps.id = id_cliente;
        strcpy(ps.cpf, cpfnpj);

        temp = InserirCpf(lcp, ps);
    } else {
        cl.tipo = TIPO_PESSOA_JURIDICA;

        PessoaJuridica ps;
        ps.id = id_cliente;
        strcpy(ps.cnpj, cpfnpj);

        temp = InserirCnpj(lcn, ps);
    }
    if(!temp) {
        ligarBlur(janela, janela_backup);

        mostrarAlerta(true, "Não foi possível adicionar o CPF/CNPJ, verifique o uso da memória RAM ou reporte para um dos desenvolvedores");

        desligarBlur(janela, janela_backup);

        delwin(janela);
        refresh();
        return;
    }

    temp = InserirCliente(cl, clt);
    if(!temp) {
        ligarBlur(janela, janela_backup);

        mostrarAlerta(true, "Não foi possível adicionar o cliente, verifique o uso da memória RAM ou reporte para um dos desenvolvedores");
        
        desligarBlur(janela, janela_backup);

        delwin(janela);
        refresh();
        return;
    }

    // Finalizando
    delwin(janela);
    refresh();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubMenuRemoverPedido(ListaPedido* lp) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Remoção de pedido"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 17); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // Entrada
    int id_pedido;
    bool id_p_analise=true;
    while(id_p_analise) {
        id_pedido = scanNum(janela, 2, "ID do pedido");

        if(id_pedido == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_p_analise = analisarPedido(lp, id_pedido);
        if(id_p_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    int tipo = scanOpcoesExclusao(janela, 5);
    if(tipo == -1) {
        delwin(janela);
        refresh();
        return;
    }

    if(tipo) {
        removerPedido(lp, id_pedido);
    }

    refresh();
    wrefresh(janela);

    // Finalizando
    delwin(janela);
    refresh();
}

void mostrarSubMenuRemoverProduto(ListaProduto* lpr, ListaPedido* lp) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Remoção de produto"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};
    bool temp;

    // Backup da janela
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 18); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // Entrada
    int id_produto;
    bool id_p_analise=false;
    while(!id_p_analise) {
        id_produto = scanNum(janela, 2, "ID do produto");

        if(id_produto == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_p_analise = analisarProduto(lpr, id_produto);
        if(!id_p_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    int tipo = scanOpcoesExclusao(janela, 5);
    if(tipo == -1) {
        delwin(janela);
        refresh();
        return;
    }

    if(tipo) {
        temp = removerProduto(lpr, lp, id_produto);
        if(!temp) {
            ligarBlur(janela, janela_backup);
            mostrarAlerta(true, "Não foi possível excluir o produto, verifique se há algum pedido com o respectivo produto em aberto.\n\nAperte qualquer tecla para continuar...");
            desligarBlur(janela, janela_backup);
        }
    }

    refresh();
    wrefresh(janela);

    // Finalizando
    delwin(janela);
    refresh();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubMenuRemoverCliente(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Remoção de cliente"; // Título da janela
    char opcoes[4][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 18); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // Entrada
    int id_cliente;
    bool id_p_analise=true;
    while(id_p_analise) {
        id_cliente = scanNum(janela, 2, "ID do cliente");

        if(id_cliente == -1) {
            delwin(janela);
            refresh();
            return;
        }

        id_p_analise = verificar_id_cliente(clt, id_cliente);
        if(id_p_analise) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }

    int tipo = scanOpcoesExclusao(janela, 5);
    if(tipo == -1) {
        delwin(janela);
        refresh();
        return;
    }

    if(tipo) {
        removerCliente(clt, lcp, lcn, id_cliente);
    }

    refresh();
    wrefresh(janela);

    // Finalizando
    delwin(janela);
    refresh();
}

void mostrarSubMenuConsultarProduto(ListaProduto* lpr) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Consultar produto"; // Título da janela
    char opcoes[6][50] = {"Esc", "Cancelar", "↵ ", "Continuar"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 17); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // Entrada
    int id_produto;
    Produto* produto = NULL;
    while(produto == NULL) {
        id_produto = scanNum(janela, 2, "ID do produto");

        if(id_produto == -1) {
            delwin(janela);
            refresh();
            return;
        }

        produto = consultarProduto(lpr, id_produto);
        if(produto == NULL) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não existe");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }
    mvwhline(janela, 2, 2, ' ', wj-4);
    mvwhline(janela, 3, 2, ' ', wj-4);

    // Imprimindo o produto correspondente
    wattron(janela, A_BOLD);
    mvwprintw(janela, 2, 2, "ID: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%d", (*produto).id);
    
    wattron(janela, A_BOLD);
    mvwprintw(janela, 4, 2, "Descrição: ");
    wattroff(janela, A_BOLD);
    printTextoLim(janela, 5, 2, (*produto).descricao, wj-4);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 7, 2, "Preço: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%.2lf reais", (*produto).preco);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 9, 2, "Estoque: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%d", (*produto).estoque);

    refresh();
    wrefresh(janela);
    int entr = wgetch(janela);
    while(entr != 27 && entr != 10){
        entr = wgetch(janela);
    }

    // ... e finalizando
    delwin(janela);
    refresh();
}

void mostrarSubMenuConsultarPedido(ListaPedido* lp) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Consultar pedido"; // Título da janela
    char opcoes[6][50] = {"Esc", "Cancelar", "↵ ", "Continuar", "i", "Mostrar lista de itens"};

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 16); // Título
    printOpcoesJanela(janela, opcoes, 6); // Opções

    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // ID
    int id_pd;
    pedido* pd = NULL;
    while(pd == NULL) {
        id_pd = scanNum(janela, 2, "ID do pedido");

        if(id_pd == -1) {
            delwin(janela);
            refresh();
            return;
        }

        pd = consultarPedido(lp, id_pd);
        if(pd == NULL) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não existe");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }
    mvwhline(janela, 2, 2, ' ', wj-4);
    mvwhline(janela, 3, 2, ' ', wj-4);

    // Imprimindo o produto correspondente
    wattron(janela, A_BOLD);
    mvwprintw(janela, 2, 2, "ID do pedido: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%d", (*pd).id);
    
    wattron(janela, A_BOLD);
    mvwprintw(janela, 4, 2, "ID do cliente: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%d", (*pd).clienteId);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 6, 2, "Data: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%s", (*pd).data);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 8, 2, "Total: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%.2lf", (*pd).total);

    refresh();
    wrefresh(janela);

    int entr;
    while(entr != 27 && entr != 10) {
        entr = wgetch(janela);

        if(entr == 105 || entr == 73) {
            ligarBlur(janela, janela_backup);

            mostrarSubSubMenuMostrarItPedido(pd);

            desligarBlur(janela, janela_backup);
            printOpcoesJanela(janela, opcoes, 6); // Opções de novo, tem um bug específico
        }
    }

    // ... e finalizando
    delwin(janela);
    refresh();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarSubMenuConsultarCliente(ListaCliente* clt, ListaCpf* cpfs, ListaCnpj* cnpjs) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10-2, wj = w-w/10-2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Consultar cliente"; // Título da janela
    char opcoes[6][50] = {"Esc", "Cancelar", "↵ ", "Continuar"}; // Opções da janela

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 17); // Título
    printOpcoesJanela(janela, opcoes, 4); // Opções

    // Entrada
    int id_cliente;
    cliente* cl = NULL;
    while(cl == NULL) {
        id_cliente = scanNum(janela, 2, "ID do cliente");

        if(id_cliente == -1) {
            // Finalizando
            delwin(janela);
            refresh();
            return;
        }

        cl = consultarCliente(clt, id_cliente);
        if(cl == NULL) {
            wattron(janela, COLOR_PAIR(4));
            mvwprintw(janela, 4, 2, "O ID não está cadastrado");
            wattroff(janela, COLOR_PAIR(4));
        } else {
            mvwhline(janela, 4, 2, ' ', wj-4);
        }

        refresh();
        wrefresh(janela);
    }
    mvwhline(janela, 2, 2, ' ', wj-4);
    mvwhline(janela, 3, 2, ' ', wj-4);

    // Imprimindo o produto correspondente
    wattron(janela, A_BOLD);
    mvwprintw(janela, 2, 2, "ID: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%d", (*cl).id);
    
    wattron(janela, A_BOLD);
    mvwprintw(janela, 4, 2, "Nome: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%s", (*cl).nome);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 6, 2, "Tipo: ");
    wattroff(janela, A_BOLD);
    if(!(*cl).tipo) {
        wprintw(janela, "Pessoa física");

        wattron(janela, A_BOLD);
        mvwprintw(janela, 7, 2, "CPF: ");
        wattroff(janela, A_BOLD);
        wprintw(janela, "%s", (*((*cpfs).cpfs+buscar_cpf((*cl).id, cpfs))).cpf);
    } else {
        wprintw(janela, "Pessoa jurídica");

        wattron(janela, A_BOLD);
        mvwprintw(janela, 7, 2, "CNPJ: ");
        wattroff(janela, A_BOLD);
        wprintw(janela, "%s", (*((*cnpjs).cnpjs+buscar_cnpj((*cl).id, cnpjs))).cnpj);
    }

    wattron(janela, A_BOLD);
    mvwprintw(janela, 9, 2, "Telefone: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%s", (*cl).telefone);

    wattron(janela, A_BOLD);
    mvwprintw(janela, 11, 2, "Endereço: ");
    wattroff(janela, A_BOLD);
    wprintw(janela, "%s", (*cl).endereco);

    refresh();
    wrefresh(janela);
    
    int entr = wgetch(janela);
    while(entr != 27 && entr != 10) {
        entr = wgetch(janela);
    }

    // ... e finalizando
    delwin(janela);
    refresh();
}

// =========== Menus ===========

void mostrarMenuClientes(ListaCliente* clt, ListaCpf* lcp, ListaCnpj* lcn) { //
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10, wj = w-w/10; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Menu de clientes"; // Título da janela
    char opcoes[2][50] = {"Esc", "Sair/voltar"};
    char opcoes_menu[4][50] = {"> Lista de clientes", "Consultar", "Cadastrar", "Remover"};
    
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 16); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções da janela

    // Seleção do menu
    int entr=0;
    while(true) {
        entr = mostrarOpcoesMenuUni(janela, opcoes_menu, 4, entr);

        switch(entr) {
            case 0:
                mvwprintw(janela, 2, 2, "v Lista de clientes");
                mostrarListaClientes(janela, 3, clt, janela_backup, lcp, lcn);
                break;
            case 1:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuConsultarCliente(clt, lcp, lcn);

                desligarBlur(janela, janela_backup);
                break;
            case 2:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuCadastrarCliente(clt, lcp, lcn);

                desligarBlur(janela, janela_backup);
                break;
            case 3:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuRemoverCliente(clt, lcp, lcn);

                desligarBlur(janela, janela_backup);
                break;
            case 27:
                delwin(janela);
                refresh();
                return;
        }

        refresh();
        wrefresh(janela);
    }

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarMenuProdutos(ListaProduto* lpr, ListaPedido* lp) { //
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10, wj = w-w/10; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Menu de produtos"; // Título da janela
    char opcoes[2][50] = {"Esc", "Sair/voltar"};
    char opcoes_menu[4][50] = {"> Lista de produtos", "Consultar", "Cadastrar", "Remover"};
    
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 16); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções da janela

    // Seleção do menu
    int entr=0;
    while(true) {
        entr = mostrarOpcoesMenuUni(janela, opcoes_menu, 4, entr);

        switch(entr) {
            case 0:
                mvwprintw(janela, 2, 2, "v Lista de produtos");
                mostrarListaProdutos(janela, 3, lpr);
                break;
            case 1:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuConsultarProduto(lpr);

                desligarBlur(janela, janela_backup);
                break;
            case 2:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuCadastrarProduto(lpr);

                desligarBlur(janela, janela_backup);
                break;
            case 3:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuRemoverProduto(lpr, lp);

                desligarBlur(janela, janela_backup);
                break;
            case 27:
                delwin(janela);
                refresh();
                return;
        }
    }

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarMenuPedidos(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/10, wj = w-w/10; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char titulo[50] = "Menu de pedidos"; // Título da janela
    char opcoes[2][50] = {"Esc", "Sair/voltar"};
    char opcoes_menu[4][50] = {"> Lista de pedidos", "Consultar", "Cadastrar", "Remover"};
    
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão
    printTituloJanela(janela, titulo, 15); // Título
    printOpcoesJanela(janela, opcoes, 2); // Opções

    // Seleção do menu
    int entr=0;
    while(true) {
        entr = mostrarOpcoesMenuUni(janela, opcoes_menu, 4, entr);

        switch(entr) {
            case 0:
                mvwprintw(janela, 2, 2, "v Lista de pedidos");
                mostrarListaPedidos(janela, 3, lp, lpr, janela_backup);
                break;
            case 1:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuConsultarPedido(lp);

                desligarBlur(janela, janela_backup);
                break;
            case 2:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuCadastrarPedido(lp, lc, lpr);

                desligarBlur(janela, janela_backup);
                break;
            case 3:
                ligarBlur(janela, janela_backup);

                mostrarSubMenuRemoverPedido(lp);

                desligarBlur(janela, janela_backup);
                break;
            case 27:
                delwin(janela);
                refresh();
                return;
        }
    }

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}

void mostrarMenuPrincipal(ListaPedido* lp, ListaCliente* lc, ListaProduto* lpr, ListaCpf* lcp, ListaCnpj* lcn) {
    // Váriaveis
    int h, w; getmaxyx(stdscr, h, w); // Altura e largura da janela stdscr
    int hj = h-h/3, wj = w-w/2; // Altura e largura da janela desse menu
    int yj = h/2-hj/2, xj = w/2-wj/2; // Localização (x,y) da janela desse menu
    char opcoes[4][50] = {"Clientes", "Produtos", "Pedidos", "Sair"}; // Opções
    int t_op[4] = {8, 8, 8, 4}; // Tamanho das strings (Usar para centralização)
    
    int** janela_backup = (int**)malloc(hj*sizeof(int*));
    for(int i = 0; i < hj; i++) {
        janela_backup[i] = (int*)malloc(wj*sizeof(int));
    }

    // Criando janela e aplicando suas configurações
    WINDOW* janela = newwin(hj, wj, yj, xj);
    keypad(janela, true); // Uso de teclas especiais
    box(janela, 0, 0); // Contorno padrão

    // Desenhando o título
    mvwprintw(janela, hj/4-2, wj/2-10, "  ___    ___   ___ ");
    mvwprintw(janela, hj/4-1, wj/2-10, " / __/  / __| | _ |");
    mvwprintw(janela, hj/4, wj/2-10,   " \\__ | | (__  |  _/");
    mvwprintw(janela, hj/4+1, wj/2-10, " /___/  \\___| |_|  ");

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

        switch(entr) {
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

                ligarBlur(janela, janela_backup);

                switch(sel)
                {
                    case 0: // Cliente
                        mostrarMenuClientes(lc, lcp, lcn);
                        break;
                    case 1: // Produto
                        mostrarMenuProdutos(lpr, lp);
                        break;
                    case 2: // Pedido
                        mostrarMenuPedidos(lp, lc, lpr);
                        break;
                    case 3: // Fechar a aplicação
                        return;
                        break;
                }

                clear();
                desligarBlur(janela, janela_backup);
        }
    }

    // Versão do sistema
    //wattron(janela, COLOR_PAIR(3));
    //mvwprintw(janela, hj-2, (wj-1)-5, "Alpha");
    //wattroff(janela, COLOR_PAIR(3));

    refresh();
    wrefresh(janela);
    getch();

    // Liberando o ponteiro
    for(int i = 0; i < hj; i++) {
        free(janela_backup[i]);
    }
    free(janela_backup);
}