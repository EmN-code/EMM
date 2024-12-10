
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    // Simulate turning on the LED
    printf("LED turned on\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set the timer to trigger every second
    evtimer_add_msg(&evtimer, timex_set(TIMER_INTERVAL, 0), NULL, timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
