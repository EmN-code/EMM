
#include <stdlib.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered!\n");
    // Assuming LED is connected to GPIO 5
    gpio_toggle(GPIO_PIN(0, 5));
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer, &mbox);

    // Set up the timer to trigger every second
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
        msg_t msg;
        mbox_get(&mbox, &msg);
        // Handle the message if needed
    }

    return 0;
}
