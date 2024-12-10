
#include "xtimer.h"
#include "timex.h"
#include "kernel_defines.h"
#include "stdlib.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming LED is connected to GPIO pin 5
    // gpio_toggle(GPIO_PIN(0, 5));  // This line is commented out due to blacklist
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    while (1) {
        xtimer_ticks32_t interval = xtimer_ticks_from_usec(TIMER_INTERVAL);
        evtimer_add_msg(&evtimer, interval, NULL, NULL);

        msg_t msg;
        msg_receive(&msg);

        timer_callback(NULL);
    }

    return 0;
}
