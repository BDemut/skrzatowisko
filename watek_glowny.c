#include "main.h"
#include "request.h"
#include "lamport.h"
#include "communication.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    for (int i=0; i<2; i++) {
        int w = random()%W;

        packet_t *pkt = malloc(sizeof(packet_t));
        pkt->w = w;
        
        int ts = lamportEvent();
        debug("TS: %u Wysylam request o %d wstazek", ts, w);
        for (int i=0; i<size; i++) {
            sendPacket( pkt, i, REQ, ts);
        }
        
        debug("TS: %u Czekam na wejscie do sekcji", getTimestamp());
        while (!hasRequest() || !canEnter()) {}

        debug("TS: %u Wchodze do sekcji", getTimestamp());
        sleep(1);

        ts = lamportEvent();
        debug("TS: %u Wysylam release", ts);
        for (int i=0; i<size; i++) {
            sendPacket(0, i, RELEASE, ts);
        }

        debug("TS: %u Czekam na release", getTimestamp());
        while (hasRequest()) {}
    }
}
