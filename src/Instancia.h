#ifndef INSTANCIA_H
#define INSTANCIA_H

#define CAMINHO_PADRAO "./instancias/I1_I10/1I1"

#include <iostream>
#include <fstream>
#include <vector>
#include "Item.h"

using std::vector;
using std::string;
using std::ifstream; 
using std::cout;
using std::endl;

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
    Instancia(int nItens, float pesoMax, vector<Item> itens); // Instancia para CPLEX
    ~Instancia();

    void printInstancia(bool printarItens=false);
};

#endif