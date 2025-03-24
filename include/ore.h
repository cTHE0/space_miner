#ifndef ORE_H
#define ORE_H

typedef enum { FUEL, ORE1, ORE2, ORE3, EMPTY } Ore; //Types de minerais

typedef struct {
    Ore ore;
    int capacity;
} Compartment;

#endif