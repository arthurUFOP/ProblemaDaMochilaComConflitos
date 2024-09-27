#ifndef GRASP_H
#define GRASP_H

#include "Solucao.h"
#include "Instancia.h"
#include "Metaheuristica.h"
#include "BuscasLocais.h"
#include "MetodosExatos.h"

// É um GRASP para 
class Grasp : public Metaheuristica
{
private:
    float alpha;
    int graspMax;
    pair<Solucao, Solucao> construcao(Instancia& inst); // Retorna a solucao e seu CI
    BuscaLocalDeCI* bl;

public:
    Grasp();
    Grasp(float alpha, int graspMax);
    ~Grasp();
    Solucao gerarSolucao(Instancia& inst);
};

#endif