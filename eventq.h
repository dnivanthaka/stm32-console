#ifndef EVENTQ_H
#define EVENTQ_H

#define EVENTQUEUE_SIZE 8

enum event_t { NOEVENT, KEYPRESS, SOUND };

typedef struct eventq_t {
    enum event_t event;
    void *data;
} eventq_t;

eventq_t events[EVENTQUEUE_SIZE];

void eventq_init();
void eventq_queue(eventq_t *event);
void eventq_dequeue(eventq_t *event);

#endif