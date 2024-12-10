#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include "event/callback.h"
#include "event/timeout.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"

#define THREAD_STACKSIZE_DEFAULT 1024

static void event_callback(void *arg) {
    puts("Periodic event triggered!");
}

int main(void) {
    msg_t mbox_queue[8];
    mbox_t mbox;
    evtimer_t evtimer;
    evtimer_mbox_event_t event;

    // Initialize the mailbox
    mbox_init(&mbox, mbox_queue, 8);

    // Initialize the event timer for mbox
    evtimer_init_mbox(&evtimer);

    // Setup event
    event.event.callback = event_callback;
    event.event.arg = NULL;
    event.exp = 1000 * 1000;
    event.period = 1000 * 1000; // 1 second
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop
    while (1) {
        msg_receive(mbox_queue);
    }

    return 0;
}