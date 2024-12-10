
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    (void)event;
    printf("Timer triggered!\n");
    // Simulate turning on the LED
    // Replace this with actual LED control code if needed
    printf("LED turned on!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set the timer to trigger every second
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        _evtimer_mbox_handler(NULL);
    }

    return 0;
}
