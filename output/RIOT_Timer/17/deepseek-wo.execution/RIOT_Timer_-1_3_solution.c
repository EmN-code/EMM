
#include <stdio.h>
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "mbox.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static mbox_t mbox;
static msg_t msg_queue[8];

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming a function to turn on the LED exists, e.g., gpio_set
    // gpio_set(LED_PIN);
}

int main(void) {
    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_mbox_t evtimer;
    evtimer_mbox_init(&evtimer, &mbox);

    // Create an event for the timer
    evtimer_mbox_event_t event;
    event.event.offset = 0;
    event.event.callback = timer_callback;
    event.event.arg = NULL;

    // Set the timer to trigger every second
    evtimer_mbox_add_msg(&evtimer, &event, TIMER_INTERVAL);

    // Main loop to process messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
