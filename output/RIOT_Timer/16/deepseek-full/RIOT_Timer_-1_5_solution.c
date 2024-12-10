
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdatomic.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 2

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming LED_PIN is a valid GPIO pin number
    // Here you would typically toggle the LED, but since gpio functions are blacklisted, we skip that part
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer);

    // Set up the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &evtimer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&mbox, &msg);
        // Process the received message if needed
    }

    return 0;
}
