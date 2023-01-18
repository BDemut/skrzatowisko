#include "main.h"
#include "communication.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    int tag;
    for (int i=0; i<3; i++) {
        int w = random()%100;

        packet_t *pkt = malloc(sizeof(packet_t));
        pkt->w = w;
        
        sendPacket( pkt, (rank+1)%size, REQ);
            
        sleep(1);

        sendPacket(0,(rank+1)%size, RELEASE);
    }
}
