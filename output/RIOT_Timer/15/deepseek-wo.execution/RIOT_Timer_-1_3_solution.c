
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stddef.h>
#include <mbox.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void led_toggle(void)
{
    // Assuming LED is connected to GPIO 21
    gpio_toggle(GPIO_PIN(0, 21));
}

static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer, timer_callback, NULL);

    // Add a periodic event to the event timer
    evtimer_add_msg(&evtimer, &(evtimer_msg_event_t){ .event = { .interval = 1 * US_PER_SEC, .callback = timer_callback } });

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        led_toggle();
    }

    return 0;
}
