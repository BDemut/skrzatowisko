#include "main.h"
#include "watek_glowny.h"

void mainLoop()
{
    srandom(rank);
    int tag;

    int w = random()%100;

    debug("Wysyłam REQ");
    packet_t *pkt = malloc(sizeof(packet_t));
    pkt->data = w;
    tag = ACK;
    
    sendPacket( pkt, (rank+1)%size, tag);
        
    sleep(1);
}
