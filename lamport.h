#ifndef LAMPORTH
#define LAMPORTH

void initLamport(int size);
int lamportEvent();
int lamportEventExt(int externalTimestamp, int externalId);
int getTimestampOf(int id);
void finalizeLamport();

#endif