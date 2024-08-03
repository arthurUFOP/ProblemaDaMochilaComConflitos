#ifndef ITEM_H
#define ITEM_H

#include <iostream>

using std::ostream;

typedef struct sItem
{
    int index;
    float valor;
    float peso;

    friend ostream& operator<<(ostream& os, const sItem& it);
} Item;

#endif