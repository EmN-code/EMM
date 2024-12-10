
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t periodic_timer;
static msg_t periodic_msg;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the periodic timer
    periodic_msg.content.ptr = NULL;
    periodic_msg.type = 0;

    evtimer_init_msg(&periodic_timer);

    // Set up the periodic event
    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now();
    timer.callback = (xtimer_callback_t)periodic_callback;

    // Start the periodic event
    while (1) {
        xtimer_set(&timer, PERIODIC_INTERVAL);
        msg_receive(&periodic_msg);
    }

    return 0;
}
