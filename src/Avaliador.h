#ifndef AVALIADOR_H
#define AVALIADOR_H

#include "Instancia.h"
#include "Solucao.h"

float avaliaFO(Instancia& inst, Solucao& sol);
float avaliaPeso(Instancia& inst, Solucao& sol);
bool avaliaValidade(Instancia& inst, Solucao& sol);

#endif 