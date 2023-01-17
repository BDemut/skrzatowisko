#include "main.h"
#include "util.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    for (int i=0; i<3; i++) {
        debug("czekam na recv");
        recievePacket();
    }
}

