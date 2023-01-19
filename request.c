#include "request.h"
#include <stdlib.h>
#include "main.h"

request_t* requestQueue;
pthread_mutex_t mutex;

void initRequestQueue(int size) {
    pthread_mutex_init(&mutex, NULL);
    requestQueue = malloc(sizeof(request_t) * size);

    for (int i=0; i<size; i++) {
        requestQueue[i] = noRequest();
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
    pthread_mutex_lock(&mutex);
    for (int i = size - 1; i>=0; i--) {
        if (requestQueue[i].id == EMPTY_ID || requestQueue[i].timestamp > req.timestamp) {
            requestQueue[i] = requestQueue[i-1];
        } else if (requestQueue[i].timestamp < req.timestamp) {
            requestQueue[i+1] = req;
            break;
        } else {
            if (req.id < requestQueue[i].id) {
                requestQueue[i] = requestQueue[i-1];
            } else {
                requestQueue[i+1] = req;
                break;
            }
        }

        if (i == 0) {
            requestQueue[0] = req;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

// true jesli usunieto request, false jesli nie bylo requesta o danym id
int removeFromQueue(int id) {
    pthread_mutex_lock(&mutex);
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
    pthread_mutex_unlock(&mutex);
    return requestRemoved;
}

request_t findRequest(int id) {
    for (int i = 0; i< size; i++) {
        if (requestQueue[i].id == id) {
            return requestQueue[i];
        }
    }
    return noRequest();
}

int findRequestPosition(int id) {
    for (int i = 0; i< size; i++) {
        if (requestQueue[i].id == id) {
            return i;
        }
    }
    return -1;
}

int canEnter() {
    pthread_mutex_lock(&mutex);
    int claimedHorses = 0;
    int claimedRibbons = 0;
    int dwarvesWithHigherTimestampOrBetterRequest = 0;
    int ret = FALSE;

    request_t myRequest = findRequest(rank);
    int myRequestPosition = findRequestPosition(rank);

    request_t request;
    int requestPosition;
    for (int i = 0; i<size; i++) {
        request = findRequest(i);
        requestPosition = findRequestPosition(i);

        if (requestPosition <= myRequestPosition && requestPosition != -1) {
            claimedHorses++;
            claimedRibbons += request.w;
            dwarvesWithHigherTimestampOrBetterRequest++;
        } else {
            if (getTimestampOf(i) > myRequest.timestamp)
                dwarvesWithHigherTimestampOrBetterRequest++;
        }
    }
    pthread_mutex_unlock(&mutex);

    return claimedHorses <= K && claimedRibbons <= W && dwarvesWithHigherTimestampOrBetterRequest == size;
}

int hasRequest() {
    pthread_mutex_lock(&mutex);
    int ret = FALSE;
    for (int i = 0; i< size; i++) {
        if (requestQueue[i].id == rank) {
            ret = TRUE;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    return ret;
}

void finalizeRequestQueue() {
    free(requestQueue);
    pthread_mutex_destroy(&mutex);
}