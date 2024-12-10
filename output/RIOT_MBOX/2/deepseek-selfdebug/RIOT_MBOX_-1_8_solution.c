
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"
#include "msg.h"

#define INTERVAL (1000000U)  // 1 second interval

static msg_t msg_queue[16];

static void periodic_event_handler(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_thread_queue, periodic_event_handler, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
