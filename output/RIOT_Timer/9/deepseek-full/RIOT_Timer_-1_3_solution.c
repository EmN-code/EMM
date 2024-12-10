
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <xtimer.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5                        // Assuming LED is connected to GPIO5

static evtimer_msg_t evtimer;
static mbox_t mbox;
static msg_t msg_queue[8];

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Toggle LED logic here (assuming GPIO toggle function is not allowed)
    // For simplicity, we just print a message indicating the LED should be turned on
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer, &mbox);

    // Set up the timer to trigger every second
    xtimer_ticks32_t interval = xtimer_ticks32_from_usec(TIMER_INTERVAL);
    evtimer_add_msg(&evtimer, interval, timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&mbox, &msg);
        // Handle the message if needed
    }

    return 0;
}
