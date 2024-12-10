
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdatomic.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, mbox_pid(&mbox));
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &evtimer, TIMER_INTERVAL, timer_callback, NULL);

    msg_t msg;
    while (1) {
        // Wait for a message in the mailbox
        msg_receive(&msg);

        // Toggle the LED (assuming LED is connected to GPIO pin 5)
        // Note: This part is commented out due to the blacklist restrictions
        // gpio_toggle(GPIO_PIN(0, 5));

        // For demonstration purposes, print a message instead
        printf("Timer event received, LED toggled (simulated)\n");
    }

    return 0;
}
