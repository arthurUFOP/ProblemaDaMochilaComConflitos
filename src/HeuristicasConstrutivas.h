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
#include "MetodosExatos.h"
#include "Comparadores.h"

using std::map;
using std::sort;
using std::vector;
typedef map<int, int> ParametrosExtra;

Solucao backtrackComIteracaoMaxima(Instancia& inst, Solucao& melhorSol, float* melhorFo, Solucao& solAtual, vector<Item>& itens, int i, float pesoAtual, int* iter, int iterMax, bool* continua);

// OOP - Uma classe base para heuristicas construtivas!
// Tudo que uma heuristica construtiva faz eh gerar uma solucao a partir de uma instancia
class HeuristicaConstrutiva {
public:
    virtual Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra())=0;
};

class DecomposicaoGulosa : public HeuristicaConstrutiva {
public:
    Solucao conjuntoIndependente;
    DecomposicaoGulosa();
    ~DecomposicaoGulosa();
    
    Solucao decomposicaoGulosa(Instancia& inst, ParametrosExtra pExtra);
    Solucao gerarSolucao(Instancia& inst, ParametrosExtra pExtra=ParametrosExtra());
};

class DecomposicaoAleatoria : public HeuristicaConstrutiva {
public:
    Solucao conjuntoIndependente;
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