
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

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming LED is connected to a specific GPIO pin, you can toggle it here
    // For example, if LED is connected to GPIO_PIN(0, 16), you can use:
    // gpio_toggle(GPIO_PIN(0, 16));
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_mbox(&mbox);

    // Set up the timer to trigger every second
    evtimer_add_msg(&mbox, (timex_t){TIMER_INTERVAL / US_PER_SEC, TIMER_INTERVAL % US_PER_SEC}, timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            msg.handler(msg.ptr);
        }
    }

    return 0;
}
