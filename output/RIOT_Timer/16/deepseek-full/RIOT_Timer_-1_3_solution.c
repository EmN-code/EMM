
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, 8);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer, &mbox);

    // Set up the timer to trigger every second
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            timer_callback(NULL);
        }
    }

    return 0;
}
