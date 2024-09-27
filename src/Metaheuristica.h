#ifndef METAHEURISTICA_H
#define METAHEURISTICA_H

#include "Solucao.h"
#include "Instancia.h"

class Metaheuristica
{
public:
    virtual Solucao gerarSolucao(Instancia& inst)=0;
};

#endif