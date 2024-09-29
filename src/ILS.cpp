#include "ILS.h"

ILS::ILS() {
    this->ilsMax = 6000;
    this->vezesMax = 1000;
}

ILS::ILS(int ilsMax, int vezesMax) {
    this->ilsMax = ilsMax;
    this->vezesMax = vezesMax;
}

ILS::~ILS() {}

Solucao ILS::perturbacao(Instancia& inst, Solucao& sol, int nivel) {
    Solucao solPerturbada = sol;
    for (int i=0; i<nivel; i++) {
        int idx = rand()%solPerturbada.size();
        if (solPerturbada.at(idx) == 0)
            solPerturbada.at(idx) = 1;
        else
            solPerturbada.at(idx) = 0;
    }
    return solPerturbada;
}

Solucao ILS::gerarSolucao(Instancia& inst) {
    HeuristicaConstrutiva* hc = new DecomposicaoGulosa();
    BuscaLocal* bl = new DescidaInversaoDe2aNBitsFI();

    Solucao melhorSol = hc->gerarSolucao(inst);
    melhorSol = bl->aprimorarSolucao(inst, melhorSol);
    float melhorFo = avaliaFO(inst, melhorSol);

    Solucao sol = melhorSol;
    float atualFo = melhorFo;

    int iter = 0;
    int melhorIter = iter;
    int nivel = 2; // N de bits modificados base
    int nVezes = 1;

    while (iter - melhorIter < this->ilsMax)
    {
        iter++;
        sol = this->perturbacao(inst, melhorSol, nivel);
        if (!avaliaValidade(inst, sol)) {
            if (nVezes >= this->vezesMax) {
                nivel++;
                nVezes = 1;
            }
            else
                nVezes++;

            continue;
        }

        sol = bl->aprimorarSolucao(inst, sol);
        atualFo = avaliaFO(inst, sol);

        if (atualFo > melhorFo) {
            //cout << "Melhoria ILS no nivel " << nivel << ": " << melhorFo << " --> " << atualFo << endl;
            melhorSol = sol;
            melhorFo = atualFo;
            melhorIter = iter;
            nivel = 2;
            nVezes = 1;
        }
        else {
            if (nVezes >= this->vezesMax) {
                nivel++;
                nVezes = 1;
            }
            else
                nVezes++;
        }
    }
    return melhorSol;
}