#ifndef HEURISTICAS_CONSTRUTIVAS_H
#define HEURISTICAS_CONSTRUTIVAS_H

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Instancia.h"
#include "Solucao.h"
#include "Item.h"
#include "Avaliador.h"

using std::map, std::sort, std::vector;
typedef map<int, int> ParametrosExtra;

// OOP - Uma classe base para heuristicas construtivas!
// Tudo que uma heuristica construtiva faz eh gerar uma solucao a partir de uma instancia
class HeuristicaConstrutiva {
public:
    virtual Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra())=0;
};

class DecomposicaoGulosa : public HeuristicaConstrutiva {
public:
    DecomposicaoGulosa();
    ~DecomposicaoGulosa();
    
    Solucao decomposicaoGulosa(Instancia& inst, ParametrosExtra pExtra);
    Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra());
};

class DecomposicaoAleatoria : public HeuristicaConstrutiva {
public:
    DecomposicaoAleatoria();
    ~DecomposicaoAleatoria();
    
    Solucao decomposicaoAleatoria(Instancia& inst);
    Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra());
};

class InsercaoGulosa : public HeuristicaConstrutiva {
public:
    InsercaoGulosa();
    ~InsercaoGulosa();

    Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra());
};

class InsercaoAleatoria : public HeuristicaConstrutiva {
public:
    InsercaoAleatoria();
    ~InsercaoAleatoria();

    Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra());
};


#endif