#include <stdio.h>
#include <math.h>
#include <string.h>

void limp(char *s); // funcao limpeza de buffer depois de ler string
void limp2(); // funcao limpeza de buffer antes de ler string

void escreve(); // funcao escrever no arquivo

void ler(); // funcao ler no arquivo

int main()
{
    int escolha;

    printf("\n\n1 - Escrever no arquivo\n2 - Ler arquivo\n0 - Encerrar\nEscolha: ");
    scanf("%d", &escolha);
    limp2();
    
    while(escolha != 0)
    {
        switch(escolha)
        {
            case 1: escreve();
            break;

            case 2: ler();
            break;

            default:
            printf("Escolha uma opcao valida.");
        }

        printf("\n\n1 - Escrever no arquivo\n2 - Ler arquivo\n0 - Encerrar\nEscolha: ");
        scanf("%d", &escolha);
        limp2();

    }

    printf("\n\nFIM\n");

    return 0;
}

/* 
---------------------------------------------------------------------------
-----------------------------FIM DO INT MAIN ------------------------------
-------------------------------FUNCOES-------------------------------------
---------------------------------------------------------------------------
*/



void escreve()
{
    FILE *pt;
    char e[20];

    printf("\nO que voce vai escrever:\n");
    fgets(e, 20, stdin);
    limp(e);

    pt = fopen("C:\\Users\\Eudelcio\\Documents\\VsCode\\C\\files\\filetest.txt", "w");
    fprintf(pt, e);
    fclose(pt);

}

void ler()
{
    FILE *pt;
    char r[20];

    pt = fopen("C:\\Users\\Eudelcio\\Documents\\VsCode\\C\\files\\filetest.txt", "r");

    // fazer a leitura so se o arquivo existir
    if(pt != NULL)
    {
        printf("\nLENDO ARQUIVO:\n");
        fgets(r, 20, pt);
        //limp(r);

        printf("%s", r);
    } else {
        printf("\nErro na leitura do arquivo\n");
    }
    

    fclose(pt);

    /*
    obs. era desnecessario colocar limp(r) aqui, pois todos os problemas relacionados ao '\n' e ao buffer ja sao corrigidos antes, entao quando ler chama limp e a string e muito grande, a codiçao do while dentro de limp e verddeira, mas como nao existe nenhum \n no buffer (que esta limpo), o getchar fica esperando eu digitar algo, por isso a necessidade do duplo ENTER
    */
}


void limp(char *s)
{
    // se a string for menor que o tamanho definido
    s[strcspn(s,"\n")] = '\0';

    // depois tento alterar isso
    if(strlen(s) == 19)
    {
        int l;
        while((l = getchar()) != '\n' && l != EOF );
    }
}

void limp2()
{
    int l;
    while((l = getchar()) != '\n' && l != EOF );
}