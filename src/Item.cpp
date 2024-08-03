#include "Item.h"

ostream& operator<<(ostream& os, const sItem& it)
{
    os << "(Item " << it.index+1 << ", Valor " << it.valor << ", Peso " << it.peso << ")";
    return os;
}