#include <stdio.h>
#include <string.h>
#include "msg.h"
#include "event.h"
#include "ztimer.h"

#define TIMER_INTERVAL     (1000000U)  // Interval in microseconds
#define MSG_QUEUE_SIZE     (8U)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    evtimer_t evtimer;
    evtimer_event_t event;

    ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
   
    evtimer_set(&evtimer, &event, TIMER_INTERVAL, event_callback, NULL);
    evtimer_add(&evtimer);
   
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}