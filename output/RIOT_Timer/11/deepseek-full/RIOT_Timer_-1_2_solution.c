
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered, toggling LED\n");
    // Toggle LED logic here
}

int main(void)
{
    printf("Starting LED toggle example\n");

    // Initialize the timer
    evtimer_msg_t evtimer;
    evtimer_msg_init_mbox(&evtimer, NULL);

    // Set up the timer to trigger every second
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, 1 * US_PER_SEC);

    // Main loop to handle events
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
