#include "Solucao.h"

void imprimeSol(Solucao& sol) {
    cout << "[";
    for(int i=0; i<sol.size(); i++)
    {
        cout<< sol.at(i);
        if(i<(sol.size()-1))
            cout<< ", ";
    }
    cout<< "]";
}