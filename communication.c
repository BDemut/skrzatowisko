#include "communication.h"
#include "main.h"

// z szkieletu
void inicjuj_typ_pakietu()
{
    int       blocklengths[NITEMS] = {1,1,1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[NITEMS]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, w);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);

    MPI_Type_commit(&MPI_PAKIET_T);
}

void sendPacket(packet_t *pkt, int destination, int tag)
{
    int freepkt=0;
    if (pkt==0) { pkt = malloc(sizeof(packet_t)); freepkt=1;}
    pkt->src = rank;
    // mutex na C?
    C++;
    pkt->ts = C;
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    switch ( tag ) {
        case ACK: 
                debug("Wysylam ack do %d z timestamp %u", destination, pkt->ts);
        break;
        case RELEASE: 
                debug("Wysylam release do %d z timestamp %u", destination, pkt->ts);
        break;
        case REQ: 
                debug("Wysylam request do %d z timestamp %u, o %d wstazek", destination, pkt->ts, pkt->w);
        break;
        default:
        break;
    }
    if (freepkt) free(pkt);
}

void recievePacket()
{
    packet_t pakiet;
    MPI_Status status;
    MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //mutex na c?
    timestampVector[pakiet.src] = pakiet.ts;
    if (pakiet.ts > C) {
        C = pakiet.ts;
    }
    C++;
    switch ( status.MPI_TAG ) {
        case ACK: 
                debug("Dostałem ack od %d z timestamp %u",pakiet.src, pakiet.ts);
        break;
        case RELEASE: 
                debug("Dostałem release od %d z timestamp %u",pakiet.src, pakiet.ts);
                if (!removeFromQueue(pakiet.src)) {
                    debug("Blad podczas usuwania z kolejki!");
                } 
        break;
        case REQ: 
                debug("Dostałem request od %d z timestamp %u, o %d wstazek", pakiet.src, pakiet.ts, pakiet.w);
                request_t req;
                req.id = pakiet.src;
                req.w = pakiet.w;
                req.timestamp = pakiet.ts;
                addToQueue(req);
                sendPacket(0, pakiet.src, ACK);

        break;
        default:
        break;
    }
}
