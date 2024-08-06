#ifndef METODOS_EXATOS_H
#define METODOS_EXATOS_H

#include "Solucao.h"
#include "Instancia.h"

class MetodoExato {
public:
    virtual Solucao gerarSolucaoOtima(Instancia& inst)=0;
};

// Gera a solucao otima para o problema da mochila 0-1 convencional.
class CPLEXMochilaBasico {
private:
    float tolerancia;
    bool verbose;
    int verdadeiroTamanho;

public:
    CPLEXMochilaBasico();
    CPLEXMochilaBasico(int verdadeiroTamanho, float tolerancia=0, bool verbose=true);
    ~CPLEXMochilaBasico();
    Solucao gerarSolucaoOtima(Instancia& inst);
};

#endif