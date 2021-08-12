#include "types.h"
#include "stm32f103xxx.h"
#include "eventq.h"

static int end = -1;

void eventq_init()
{
    for(uint8_t i=0;i<8;i++){
        events[i].event = NOEVENT;
        events[i].data = '\0';
    }

    end = -1;
}

void eventq_queue(eventq_t *event)
{
    if(end < (EVENTQUEUE_SIZE - 1)){
        end++;
        events[end].event = event->event;
        events[end].data = event->data;
    }
}

void eventq_dequeue(eventq_t *event)
{
    if(end >= 0){
        event->event = events[end].event;
        event->data = events[end].data;

        events[end].event = NOEVENT;
        events[end].data = '\0';
        
        end--;
    }
}