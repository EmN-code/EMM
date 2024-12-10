
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
#include <arpa/inet.h>
#include <unistd.h>
#include <stdatomic.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered, turning LED on.");
    // Assuming LED is connected to a specific GPIO pin, you would toggle it here.
    // For example: gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Process the received message
        // For example, you could check the message type and handle accordingly
    }

    return 0;
}
