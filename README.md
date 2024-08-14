# Problema Da Mochila Com Conflitos
 a.k.a. disjunctive constrained knapsack problem (DCKP)

# Instâncias
 https://data.mendeley.com/datasets/gb5hhjkygd/1

# Setup
Para executar o projeto, é necessário ter instalado o CPLEX. Outras versões podem funcionar, mas para o projeto recomenda-se a 2211. Verifique o local de instalação (recomenda-se o default /opt/ibm/ILOG/CPLEX_Studio_Community2211/): caso não utilize o padrão, faça as alterações necessárias no makefile.

O uso de uma distribuição linux também é recomendado. Provavelmente são necessárias adaptações no código para utilização em ambiente Windows

# Execução

Dentro do repositório, crie uma pasta bin através do comando `mkdir bin`.

Em seguida, `make all` para compilar o código.

A execução pode ser feita utilizando:

- `./bin/dckp` -> para executar com a instância padrão

- `./bin/dckp caminho/para/instancia` -> para executar com uma instância específica



