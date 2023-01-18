#ifndef COMM_H
#define COMM_H
#include "main.h"

#define NITEMS 3

typedef struct {
    unsigned int ts;       /* timestamp (zegar lamporta */
    int src;  
    int w;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag, int ts);
void recievePacket();

#endif