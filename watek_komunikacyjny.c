#include "main.h"
#include "watek_komunikacyjny.h"
#include "communication.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (TRUE) {
        recievePacket();
    }
}

