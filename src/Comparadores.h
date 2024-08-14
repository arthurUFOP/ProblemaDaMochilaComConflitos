#ifndef COMPARADORES_H
#define COMPARADORES_H

#include "Item.h"

// Comparador entre itens (valor)
extern struct sCompValor {
  bool operator() (Item i,Item j) {return (i.valor<j.valor);}
} CompValor;

// Comparador entre itens (n. Restricoes, DESCENDENTE)
extern struct sCompRest {
  bool operator() (Item i,Item j) {return (i.nRest>j.nRest);}
} CompRest;

// Comparador entre itens (valor por peso, DESCENDENTE)
extern struct sCompValorPorPeso {
  bool operator() (Item i,Item j) {return ((i.valor/i.peso)>(j.valor/j.peso));}
} CompValorPorPeso;


#endif