
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdatomic.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    // Simulate turning on the LED
    printf("LED turned on\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message (in this case, just print a message)
        printf("Message received\n");
    }

    return 0;
}
