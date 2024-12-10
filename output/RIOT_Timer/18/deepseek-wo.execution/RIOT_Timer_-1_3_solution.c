
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered, turning on LED.");
    // Assuming LED is connected to GPIO pin 5
    gpio_toggle(GPIO_PIN(0, 5));
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    ztimer_set(ZTIMER_MSEC, &evtimer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the message if needed
        }
    }

    return 0;
}
