#include "Instancia.h"
#include "Solucao.h"
#include "Avaliador.h"
#include "HeuristicasConstrutivas.h"

int main() {
    Instancia inst;
    DecomposicaoGulosa heuristicaGulosa;
    Solucao sol = heuristicaGulosa.gerarSolucao(inst);
    cout << "Solucao obtida: ";
    imprimeSol(sol);
    cout << endl << "FO: " << avaliaFO(inst, sol) << endl << "Solucao Valida: " << avaliaValidade(inst, sol) << endl;
    return 0;
}