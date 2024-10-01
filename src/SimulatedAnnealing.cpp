#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing() {
    this->saMax = 100000;
    this->t0 = 50;
    this->alpha = 0.9;
}

SimulatedAnnealing::SimulatedAnnealing(int saMax, float t0, float alpha) {
    this->saMax = saMax;
    this->t0 = t0;
    this->alpha = alpha;
}

SimulatedAnnealing::~SimulatedAnnealing() {}

Solucao SimulatedAnnealing::geraVizinho(Solucao& sol) {
    Solucao vizinho = sol;
    int i;

    // Encontra um item a ser adicionado
    for (int j=0; j<10; j++) {
        i = rand()%sol.size();
        if (sol.at(i)==0) {
            vizinho.at(i) = 1;
            break;
        }
        
    }

    // Encontra um item a ser removido
    for (int j=0; j<10; j++) {
        i = rand()%sol.size();
        if (sol.at(i)==1) {
            vizinho.at(i) = 0;
            break;
        }
    }

    return vizinho;
}

Solucao SimulatedAnnealing::gerarSolucao(Instancia& inst) {
    HeuristicaConstrutiva* hc = new DecomposicaoGulosa();

    Solucao melhorSol = hc->gerarSolucao(inst);
    Solucao solAtual = melhorSol;
    float foMelhor = avaliaFO(inst, melhorSol);
    float foAtual = foMelhor;

    float temp = this->t0;

    while (temp > 1) {
        for (int i=0; i<this->saMax; i++) {
            Solucao solVizinho = this->geraVizinho(solAtual);

            // Se o vizinho gerado for valido, prossegue-se
            if (avaliaValidade(inst, solVizinho)) {
                float foVizinho = avaliaFO(inst, solVizinho);
                float delta = foVizinho - foAtual;

                if (delta > 0) {
                    solAtual = solVizinho;
                    foAtual = foVizinho;
                    if (foAtual > foMelhor) {
                        //cout << "Melhora SA encontrada em Temp = " << temp << ". " << foMelhor << " --> " << foAtual << endl;

                        foMelhor = foAtual;
                        melhorSol = solAtual;
                    }
                }
                else {
                    float x = (rand())/(float)RAND_MAX;
                    if (x < exp(delta/temp)) {
                        solAtual = solVizinho;
                        foAtual = foVizinho;
                    }
                }

            }
        }

        temp *= alpha;
    }

    return melhorSol;

}