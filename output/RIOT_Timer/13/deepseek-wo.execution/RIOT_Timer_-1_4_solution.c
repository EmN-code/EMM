
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
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate LED toggle
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the timer to trigger every second
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)_evtimer_mbox_handler;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Toggle LED logic here
            printf("LED toggled\n");
        }
    }

    return 0;
}
