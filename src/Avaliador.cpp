#include "Avaliador.h"

float avaliaFO(Instancia& inst, Solucao& sol) {
    float fo=0;
    for (int i=0; i<sol.size(); i++) 
        fo+=sol.at(i)*inst.itens.at(i).valor;
    return fo;
}

float avaliaPeso(Instancia& inst, Solucao& sol) {
    float peso=0;
    for (int i=0; i<sol.size(); i++) 
        peso+=sol.at(i)*inst.itens.at(i).peso;
    return peso;
}

bool avaliaValidade(Instancia& inst, Solucao& sol) {
    // Verifica se nao ha conflitos
    for (int i=0; i<sol.size(); i++) {

        // Se o item i esta presente na solucao...
        if (sol.at(i)) {

            // Verificarei para cada item j ...
            for (int j=0; j<inst.nItens; j++) {

                // Caso ele (j) esteja na solucao, se i e j sao incompativeis
                if (sol.at(j) && inst.restricoes[i][j]) {
                    return false; // Caso sejam, retorno falso
                }
            }

        }
    }

    // Agora verifico se o peso esta dentro dos conformes
    float peso=0;
    for (int i=0; i<sol.size(); i++) 
        peso+=sol.at(i)*inst.itens.at(i).peso;
    if (peso > inst.pesoMax)
        return false;

    // Caso ambos estejam ok, a solucao eh valida
    return true;
}