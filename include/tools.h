#ifndef TOOLS_H
#define TOOLS_H

void *supprElemList(void *list, int *nb_elem, int type_size, int i);
void *swapElements(void *list, int nb_elem, int type_size, int i, int j);
void *moveToEnd(void *list, int nb_elem, int type_size, int index);

#endif
