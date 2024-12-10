
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define LED_PIN             GPIO_PIN(0, 18)  // Example GPIO pin for LED
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

static void _led_toggle(void) {
    // Simulate LED toggle logic here
    printf("LED toggled\n");
}

static void _evtimer_mbox_handler(event_t *event) {
    (void)event;
    _led_toggle();
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the event timer to trigger every second
    evtimer_add_mbox(&evtimer, &mbox, NULL, TIMER_INTERVAL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        _evtimer_mbox_handler(NULL);
    }

    return 0;
}
