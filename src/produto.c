#include "produto.h"

typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;


void inserirProduto(Produto *pdt){
    FILE *pt;

    scanf("%d", pdt->id); // ler id
    while(getchar() != '\n'); // tirar "\n" do buffer

    fgets(pdt->descricao, 100, stdin); // ler descricao
    limparCBarraN(&pdt->descricao); // funcao com justificativa no final

    scanf("%lf", pdt->preco); // ler preco

    scanf("%d", pdt->estoque); // ler estoque

    // abrir arquivo e cadastrar produto
    pt = fopen("produto.csv", "a");

    if(pt != NULL)
    {
        fprintf(pt, "%d,%s,%lf,%d");
        printf("Dados guardados!\n");
        // ao inves de %s, poderia usar um delimitador, mas não sei qual e como 
    } else {
        printf("Erro ao abrir o arquivo!\n");
    }

    fclose(pt);
    // arquivo fechado fim
    
}

void listarProduto(){}

void editarProduto(){}

void removerProduto(){}

void salvarProdutoCSV(){}

void carregarProdutoCSV(){}



//limp() é uma funçao pra limpar o buffer, tirar os "\n" que atrapalham as proximas leituras, a funcao deveria ficar em outro lugar, isso e um rascunho
//limp() tambem serve pra tirar o "\n" do final da string, caso ela seja menor que o tamanho máximo
void limparCBarraN(char *s)
{
    // se a string for menor que o tamanho definido, o "\n" ficou na string
    s[strcspn(s,"\n")] = '\0';

    // se a string for maior que o tamanho maximo, o "\n" esta no buffer
    if(strlen(s) == 99)
    {
        int l;
        while((l = getchar()) != '\n' && l != EOF );
    }
}