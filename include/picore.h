#ifndef PICORE_H
#define PICORE_H
#include "pilisp.h"

cell *pairlis(cell *x, cell *y, cell *a);

cell *assoc(const cell *x, cell *l);

cell *apply(cell *fn, cell *x, cell *a);

cell *eval(cell *e, cell *a);

cell *evlis(cell *m, cell *a);

#endif // !PICORE_H