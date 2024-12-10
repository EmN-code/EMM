
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5                       // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static void _evtimer_msg_handler(event_t *event)
{
    evtimer_msg_event_t *ev = (evtimer_msg_event_t *)event;
    msg_t msg;
    msg.content.ptr = (void *)ev->msg.content.ptr;
    msg_send(&msg, ev->msg.sender_pid);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init(&evtimer, _evtimer_msg_handler);

    // Set up the timer event
    evtimer_msg_event_t event;
    event.event.event_callback = _evtimer_msg_handler;
    event.msg.content.ptr = (void *)LED_PIN;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Toggle the LED
            printf("Toggling LED on GPIO %d\n", (int)msg.content.ptr);
            // Assuming GPIO toggle function is available, replace with actual implementation
            // gpio_toggle((int)msg.content.ptr);
        }
    }

    return 0;
}
