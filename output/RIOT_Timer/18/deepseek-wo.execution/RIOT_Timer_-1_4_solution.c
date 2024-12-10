
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
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

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, turning on LED\n");
    // Simulate turning on LED
    // Replace this with actual LED control code if needed
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    xtimer_t timer;
    timer.callback = timer_callback;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the message if needed
        }
    }

    return 0;
}
