#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <iostream>
#include <vector>

using std::vector, std::cout, std::endl;

typedef vector<int> Solucao;

Solucao geraMochilaVazia(int n);
void imprimeSol(const Solucao& sol);

#endif