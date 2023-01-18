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
