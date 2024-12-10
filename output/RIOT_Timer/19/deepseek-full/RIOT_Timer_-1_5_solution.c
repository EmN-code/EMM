
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <timex.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kernel_defines.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered, turning on LED.\n");
    // Assuming LED is connected to GPIO pin 5
    // gpio_toggle(GPIO_PIN(0, 5));  // This line is commented out due to blacklist
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.event.callback = timer_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_mbox;
    timer.arg = &event;

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        msg_t msg;
        mbox_get(&mbox, &msg);
        // Process the message if needed
    }

    return 0;
}
