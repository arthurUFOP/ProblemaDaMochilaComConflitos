#include <time.h>
#include "Instancia.h"
#include "Solucao.h"
#include "Avaliador.h"
#include "HeuristicasConstrutivas.h"

int main() {
    srand(time(NULL));
    Instancia inst;
    HeuristicaConstrutiva* heuristicaConstrutiva = new DecomposicaoAleatoria();
    Solucao sol = heuristicaConstrutiva->gerarSolucao(inst);
    cout << "Solucao obtida: ";
    imprimeSol(sol);
    cout << endl << "FO: " << avaliaFO(inst, sol) << endl << "Solucao Valida: " << avaliaValidade(inst, sol) << endl;
    return 0;
}