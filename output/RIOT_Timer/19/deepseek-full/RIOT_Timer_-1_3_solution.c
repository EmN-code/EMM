
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <ztimer.h>
#include <stddef.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define LED_PIN 2
#define TIMER_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void led_toggle(void)
{
    // Simulate LED toggle
    printf("LED toggled\n");
}

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize event timer
    evtimer_init_mbox(&evtimer);

    // Create an event for the timer
    evtimer_mbox_event_t event;
    event.event.callback = timer_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    // Add the event to the event timer
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set the timer to trigger every second
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_mbox;
    timer.arg = &event;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.content.value == 1) {
            led_toggle();
        }
    }

    return 0;
}
