#include "main.h"
#include "watek_glowny.h"
#include "communication.h"
#include "watek_komunikacyjny.h"
#include "util.h"

const int W = 20;
const int K = 5;
int rank, size;
pthread_t threadKom, threadMon;

unsigned int* timestampVector;
request_t* requestQueue;
unsigned int C = 0;


void finalizuj()
{
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	    fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	    MPI_Finalize();
	    exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}


int main(int argc, char **argv)
{
    MPI_Status status;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);
    srand(rank);
    inicjuj_typ_pakietu(); // tworzy typ pakietu
    packet_t pkt;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    timestampVector = malloc(sizeof(unsigned int) * size);
    requestQueue = malloc(sizeof(request_t) * size);

    for (int i=0; i<size; i++) {
        requestQueue[i] = noRequest();
        timestampVector[i] = 0;
    }

    pthread_create( &threadKom, NULL, startKomWatek , 0);

    mainLoop();
    
    for (int i=0; i<size; i++) {
        debug("Timestamp %d: %u", i, timestampVector[i]);
    }
    debugQueue();
    finalizuj();
    return 0;
}

