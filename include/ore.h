#ifndef ORE_H
#define ORE_H

typedef enum { FUEL, ORE1, ORE2, ORE3, MISSILES, EMPTY } Ore;  // Types de minerais, essence, missiles stockés


typedef struct {  // Un seul compartiment
    Ore ore;
    int capacityMax;
    int currentCapacity;
    int flowSpeed;
} Compartment;


typedef struct {  // Cargaison constituée de plusieurs compartiments
    int compartmentsNumber;
    Compartment *compartmentsList;
} Cargo;

#endif