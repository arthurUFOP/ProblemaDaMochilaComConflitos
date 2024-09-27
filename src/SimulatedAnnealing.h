#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "math.h"

#include "Metaheuristica.h"
#include "HeuristicasConstrutivas.h"


class SimulatedAnnealing : public Metaheuristica
{
private:
    int saMax;
    float t0;
    float alpha;
    Solucao geraVizinho(Solucao& sol);
public:
    SimulatedAnnealing();
    SimulatedAnnealing(int saMax, float t0, float alpha);
    ~SimulatedAnnealing();
    Solucao gerarSolucao(Instancia& inst);
};


#endif