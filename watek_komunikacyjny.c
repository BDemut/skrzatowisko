#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
	debug("czekam na recv");
    MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    switch ( status.MPI_TAG ) {
    case ACK: 
    break;
    case RELEASE: 
            debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.data);
    break;
    case REQ: 
            debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.data);
    break;
    default:
    break;
    }
}

