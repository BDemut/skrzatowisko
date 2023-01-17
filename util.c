#include "main.h"
#include "util.h"
MPI_Datatype MPI_PAKIET_T;

struct tagNames_t{
    const char *name;
    int tag;
} tagNames[] = { { "ack", ACK }, { "release", RELEASE}, { "request", REQ}};

const char const *tag2string( int tag )
{
    for (int i=0; i <sizeof(tagNames)/sizeof(struct tagNames_t);i++) {
	if ( tagNames[i].tag == tag )  return tagNames[i].name;
    }
    return "<unknown>";
}
/* tworzy typ MPI_PAKIET_T
*/
void inicjuj_typ_pakietu()
{
    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    int       blocklengths[NITEMS] = {1,1,1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[NITEMS]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, w);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);

    MPI_Type_commit(&MPI_PAKIET_T);
}

/* opis patrz util.h */
void sendPacket(packet_t *pkt, int destination, int tag)
{
    int freepkt=0;
    if (pkt==0) { pkt = malloc(sizeof(packet_t)); freepkt=1;}
    pkt->src = rank;
    C++;
    pkt->ts = C;
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    switch ( tag ) {
        case ACK: 
                //debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.w);
        break;
        case RELEASE: 
                //debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.data);
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
                //debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.w);
        break;
        case RELEASE: 
                //debug("Dostałem pakiet od %d z danymi %d",pakiet.src, pakiet.data);
        break;
        case REQ: 
                debug("Dostałem request od %d z timestamp %u, o %d wstazek", pakiet.src, pakiet.ts, pakiet.w);

        break;
        default:
        break;
    }
}

request_t noRequest() {
    request_t r;
    r.w = 0;
    r.id = EMPTY_ID;
    r.timestamp = 0;
    return r;
}

void debugQueue() {
    for (int i=0; i<size; i++) {
        debug("%d: id=%d, w=%d, timestamp=%d", i, requestQueue[i].id, requestQueue[i].w, requestQueue[i].timestamp);
    }
}

void addToQueue(request_t req) {
    for (int i = size - 1; i>=0; i--) {
        if (requestQueue[i].id == EMPTY_ID || requestQueue[i].timestamp > req.timestamp) {
            requestQueue[i] = requestQueue[i-1];
        } else if (requestQueue[i].timestamp < req.timestamp) {
            requestQueue[i+1] = req;
            return;
        } else {
            if (req.id > requestQueue[i].id) {
                requestQueue[i+1] = req;
            } else {
                requestQueue[i] = req;
            }
            return;
        }

        if (i == 0) {
            requestQueue[0] = req;
            return;
        }
    }
}

// true jesli usunieto request, false jesli nie bylo requesta o danym id
int removeFromQueue(int id) {
    int requestRemoved = FALSE;
    for (int i = 0; i < size; i++) {
        if (requestQueue[i].id == id) {
            requestQueue[i] = noRequest();
            requestRemoved = TRUE;
        } 

        if (requestRemoved) {
            if (i < size-1)
                requestQueue[i] = requestQueue[i+1];
            else 
                requestQueue[i] = noRequest();
        }
    }
    return requestRemoved;
}
