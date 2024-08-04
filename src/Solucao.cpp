#include "Solucao.h"

Solucao geraMochilaVazia(int n) {
    Solucao sol;
    for (int i=0; i<n; i++)
        sol.push_back(0);
    return sol;
}

void imprimeSol(const Solucao& sol) {
    cout << "[";
    for(int i=0; i<sol.size(); i++)
    {
        cout<< sol.at(i);
        if(i<(sol.size()-1))
            cout<< ", ";
    }
    cout<< "]";
}