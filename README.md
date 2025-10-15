# Sistema de Controle de Pedidos (SCP)

Este projeto se trata de um sistema de software para controle de pedidos, clientes e produtos, é um projeto acadêmico da PUC Goiás da diciplina de Técnicas de Programação.

A linguagem utilizada será a C, com uso da biblioteca ncurses para criar uma interface TUI, e os dados serão salvos a partir de arquivos CSV.

## Visão Geral

O sistema permite gerenciar três entidades principais:

- **Clientes:** Cadastro, remoção, consulta e listagem de clientes, que podem ser tanto Pessoa Física (com validação de CPF) quanto Pessoa Jurídica (com validação de CNPJ).
    
- **Produtos:** Manutenção de um catálogo de produtos, permitindo cadastrar, remover, consultar e listar itens disponíveis para venda.
    
- **Pedidos:** Criação de pedidos associados a um cliente e compostos por um ou mais produtos do catálogo.
    

A persistência dos dados é realizada em arquivos de texto no formato CSV (`Clientes.csv`, `Produtos.csv`, `Pedidos.csv`). Ao iniciar, o sistema carrega os dados para a memória e, ao finalizar, salva todas as alterações de volta nos arquivos.

## Funcionalidades

### Módulo Cliente

- Cadastrar Cliente (Pessoa Física e Jurídica)
    
-  Validar CPF e CNPJ
    
-  Consultar Cliente por código
    
-  Remover Cliente 
    
-  Listar todos os Clientes cadastrados 

### Módulo Produto

- Cadastrar Produto 
    
-  Consultar Produto por código 
    
-  Remover Produto (com validação para não excluir produtos associados a pedidos existentes) 
    
-  Listar todos os Produtos cadastrados 

### Módulo Pedidos

- Cadastrar Pedido, associando um cliente e itens
    
-  Consultar Pedido por código
    
-  Remover Pedido e seus respectivos itens
    
-  Listar todos os Pedidos cadastrados

## Tecnologias Utilizadas

- **Linguagem:** C
    
- **Interface:** Biblioteca `ncurses` para uma interface de texto no terminal
    
- **Plataforma:** Desenvolvido prioritariamente para **Linux**

## Como Compilar e Executar

*Isso será esclarecido após iniciar o projeto...*

## Padrões internos dentro do código

Esse tópico não é direcionado aos usuários, e sim aos desenvolvedores, aqui será esclarecido os padrões que devem ser seguidas no código pelos desenvolvedores.

### Padrões de variáveis
As variáveis serão auto-explicativas, só que não devem ser muito longas, se necessário, deve-se otimizar um pouco o nome da variável

Ex.: Pessoa, Produto, …
O nome Paralelepipedo por exemplo, deve-se ser abreviado

### Uso de acentos, til, ...
Em situações que for necessário imprimir algo para o usuário, ou qualquer texto que aparecer para ele, deve-se seguir a gramatica, com acentos, til, …

## CONTRIBUIDORES

- [João Fernando Costa Umbelino](https://github.com/Boss-Bones)
    
- [Lucas Costa Vieira de Lima](https://github.com/shoriuguen)
    
- Clebio Ribeiro Lopes
