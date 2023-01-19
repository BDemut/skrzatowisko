#include "lamport.h"
#include <stdlib.h>
#include "main.h"

unsigned int C = 0;
unsigned int* timestampVector;
pthread_mutex_t lamport_mutex;

void initLamport(int size) {
    pthread_mutex_init(&lamport_mutex, NULL);
    timestampVector = malloc(sizeof(unsigned int) * size);
    for (int i=0; i<size; i++) {
        timestampVector[i] = 0;
    }
}

int lamportEvent() {
    pthread_mutex_lock(&lamport_mutex);
    int ts = ++C;
    pthread_mutex_unlock(&lamport_mutex);
    return ts;
}

int lamportEventExt(int externalTimestamp, int externalId) {
    pthread_mutex_lock(&lamport_mutex);
    timestampVector[externalId] = externalTimestamp;
    if (externalTimestamp > C) {
        C = externalTimestamp;
    }
    int ts = ++C;
    pthread_mutex_unlock(&lamport_mutex);
    return ts;
}

int getTimestampOf(int id) {
    return timestampVector[id];
}

int getTimestamp() {
    return C;
}

void finalizeLamport() {
    pthread_mutex_destroy(&lamport_mutex);
}