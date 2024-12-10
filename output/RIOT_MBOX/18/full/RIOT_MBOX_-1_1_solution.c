#include "ztimer.h"
#include "msg.h"
#include "evtimer.h"
#include <stdio.h>

#define MSG_TYPE_EVENT (0x01)
#define EVTIMER_DELAY (1U * ZTIMER_MSEC)

int main(void) {
    printf("Starting message box example on ESP32.\n");

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer, ZTIMER_MSEC);

    evtimer_set_msg(&event, EVTIMER_DELAY, thread_getpid(), MSG_TYPE_EVENT);
    evtimer_add(&evtimer, &event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_EVENT) {
            printf("Received event message.\n");
            evtimer_add(&evtimer, &event);
        }
    }

    return 0;
}