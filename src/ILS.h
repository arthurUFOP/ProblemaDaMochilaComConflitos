#ifndef ILS_H
#define ILS_H

#include "Metaheuristica.h"
#include "HeuristicasConstrutivas.h"
#include "BuscasLocais.h"
#include "Solucao.h"
#include "Instancia.h"

// É, em realidade, o smart ILS
class ILS : public Metaheuristica
{
private:
    int ilsMax;
    int vezesMax;

    Solucao perturbacao(Instancia& inst, Solucao& sol, int nivel);

public:
    ILS();
    ILS(int ilsMax, int vezesMax);
    ~ILS();
    Solucao gerarSolucao(Instancia& inst);
};

#endif