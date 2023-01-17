#ifndef UTILH
#define UTILH
#include "main.h"


typedef struct {
    int id;
    int w;
    int timestamp;
} request_t;

#define EMPTY_ID -1

/* typ pakietu */
typedef struct {
    unsigned int ts;       /* timestamp (zegar lamporta */
    int src;  
    int w;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;
/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 3

/* Typy wiadomości */
#define APP_PKT 1
#define FINISH 2

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag);
void recievePacket();
request_t noRequest();

void addToQueue(request_t req);
int removeFromQueue(int id);
void debugQueue();
#endif
