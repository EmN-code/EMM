#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <time_units.h>

#include "periph/led.h"

static evtimer_t evtimer;
static mbox_t mbox;

void timer_handler(void)
{
    led_toggle(LED0);
}

int main(void)
{
    /* Initialize mail box and event timer */
    mbox_init(&mbox, mbox_array, MBOX_SIZE);
    evtimer_init_mbox(&evtimer);

    /* Define the timer interval as 1 second */
    ztimer_t timeout = ZTIMER_MSEC;
    timeout.value = 1000;

    /* Add the timer event to the event timer */
    evtimer_add_mbox(&evtimer, &timeout, timer_handler);

    while (1) {
        /* place the CPU to sleep while waiting for interrupts */
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}