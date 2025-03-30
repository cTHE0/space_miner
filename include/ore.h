#ifndef ORE_H
#define ORE_H

typedef enum { FUEL, ORE1, ORE2, ORE3, MISSILES, EMPTY } Ore;  // Types de minerais, essence, missiles stockés

// Un seul compartiment
typedef struct {
    Ore ore;
    int capacity;
} Compartment;

// Cargaison constituée de plusieurs compartiments
typedef struct {
    int compartmentsNumber;
    Compartment *compartmentsList;
} Cargo;

#endif