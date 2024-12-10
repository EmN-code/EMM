#include "stdlib.h"
#include "i2c.h"
#include "kernel_defines.h"
#include "kernel_types.h"
#include "xtimer.h"

int main(void) {
    // Initialize the message box
    msg_t msg_queue[8];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, 8);

    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_msg_event_t msg_event;
    msg_event.msg.content.ptr = (void *)&mbox;
    evtimer_init(&evtimer, NULL);

    // Add an event to the timer
    evtimer_add(&evtimer, &msg_event);

    while(1) {
        // Wait for the event
        msg_t msg;
        if (mbox_fetch_msg(&mbox, &msg)) {
            // An event has been received
            printf("Event received!\n");
        } 

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}