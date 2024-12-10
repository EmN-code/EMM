
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <time_units.h>
#include <thread.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <stddef.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_array[1];

static void _evtimer_msg_handler(event_t *event)
{
    printf("Event triggered!\n");
}

int main(void)
{
    evtimer_init_msg(&evtimer);

    msg_t msg;
    msg.content.ptr = NULL;
    msg.type = 0;

    while (1) {
        xtimer_periodic_event_oneshot(&evtimer, TIMER_INTERVAL, &msg);
        thread_yield();
    }

    return 0;
}
