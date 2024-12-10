
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    // Simulate turning on the LED
    printf("LED turned on\n");
}

int main(void)
{
    // Initialize the message queue and mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Set up the timer to trigger every second
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        // Handle messages if needed
    }

    return 0;
}
