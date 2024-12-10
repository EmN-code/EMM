
#include <stdlib.h>
#include <msg.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void timer_handler(void *arg)
{
    (void)arg;
    printf("Timer triggered, turning on LED\n");
    // Assuming LED is connected to a GPIO pin, you can toggle it here
    // For simplicity, we just print a message
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Create an event for the timer
    evtimer_msg_event_t event;
    event.event.handler = timer_handler;
    event.event.arg = NULL;

    // Set the timer to trigger every second
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            // Handle the message if needed
        }
    }

    return 0;
}
