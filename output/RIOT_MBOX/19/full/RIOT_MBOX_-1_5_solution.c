#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <evtimer.h>
#include <dht_params.h>
#include <dht.h>

#define MESSAGE_QUEUE_SIZE (8U)
#define SLEEP_INTERVAL_SEC (1U)

static msg_t _msg_q[MESSAGE_QUEUE_SIZE];
static mbox_t _mbox;
static evtimer_t _evtimer;
static evtimer_mbox_event_t _event;

void event_handler(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    // Initialize mailbox
    mbox_init(&_mbox, _msg_q, MESSAGE_QUEUE_SIZE);
    
    // Initialize event timer for mailbox events
    evtimer_init_mbox(&_evtimer);
    
    // Configure the event
    _event.event.callback = event_handler;
    _event.event.arg = NULL;
    
    // Add the event to the evtimer
    evtimer_add_mbox(&_evtimer, &_event, &_mbox);

    // Main loop
    while (1) {
        printf("Waiting for events...\n");
        msg_receive(NULL); // Wait for messages
        xtimer_sleep(SLEEP_INTERVAL_SEC);
    }

    return 0;
}