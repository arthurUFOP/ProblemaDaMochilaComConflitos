#ifndef INSTANCIA_H
#define INSTANCIA_H

#define CAMINHO_PADRAO "./instancias/K10.txt"

#include <iostream>
#include <fstream>
#include <vector>
#include "Item.h"

using std::vector, std::string, std::ifstream, std::cout, std::endl;

// Irei utilizar como um struct, mas selecionei classe
// pela comodidade do construtor e destrutor
class Instancia
{
public:
    int nItens;
    int nRest;
    float pesoMax;
    int** restricoes;
    vector<Item> itens;
    Instancia();
    Instancia(string caminhoArquivo);
    ~Instancia();

    void printInstancia(bool printarItens=false);
};

#endif