#ifndef REQUESTH
#define REQUESTH

#define EMPTY_ID -1

typedef struct {
    int id;
    int w;
    int timestamp;
} request_t;

request_t noRequest();

void initRequestQueue(int size);
void addToQueue(request_t req);
int removeFromQueue(int id);
int canEnter();
int hasRequest();
void debugQueue();
void finalizeRequestQueue();

#endif