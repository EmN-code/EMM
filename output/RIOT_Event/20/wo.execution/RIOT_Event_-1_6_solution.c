#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "kernel_defines.h"
#include "event/thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_msg_event_t event;
static evtimer_t evtimer;

void callback(void *arg) {
    puts("Event triggered");
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the evtimer with a message-based event handler */
    evtimer_init_msg(&evtimer);

    /* Set the callback function */
    event.event.callback = callback;

    /* Schedule the event every second (1,000,000 microseconds) */
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    /* Now the evtimer is running, loop forever */
    while (1) {
        /* Let other threads work */
        thread_yield();
    }
    return 0;
}