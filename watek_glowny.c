#include "main.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    for (int i=0; i<3; i++) {
        int w = random()%100;

        packet_t *pkt = malloc(sizeof(packet_t));
        pkt->w = w;
        tag = REQ;
        
        sendPacket( pkt, (rank+1)%size, tag);
            
        sleep(1);
    }
}
