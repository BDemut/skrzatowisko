#include "lamport.h"
#include <stdlib.h>

unsigned int C = 0;
unsigned int* timestampVector;

void initLamport(int size) {
    timestampVector = malloc(sizeof(unsigned int) * size);
    for (int i=0; i<size; i++) {
        timestampVector[i] = 0;
    }
}

int lamportEvent() {
    return ++C;
}

int lamportEventExt(int externalTimestamp, int externalId) {
    timestampVector[externalId] = externalTimestamp;
    if (externalTimestamp > C) {
        C = externalTimestamp;
    }
    return ++C;
}

int getTimestamp(int id) {
    return timestampVector[id];
}