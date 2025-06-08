#ifndef ORE_H
#define ORE_H

typedef enum { FUEL, ORE1, ORE2, ORE3, ORE4, EMPTY } Ore;  // Types de minerais, essence et missiles stockes

typedef struct {  // Un seul compartiment
    Ore ore;
    Ore flowBase_in;    // Permet au joueur de gerer les entrees et sorties dans les transporteurs
    Ore flowBase_out;
    Ore flowTarget_in;
    Ore flowTarget_out;
    int currentCapacity, maxCapacity;
    int flowSpeed;
} Compartment;


typedef struct {  // Cargaison constituee de plusieurs compartiments
    int compartmentsNumber;
    Compartment *compartmentsList;
} Cargo;

#endif