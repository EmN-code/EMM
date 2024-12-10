
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "time_units.h"

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
    puts("Starting periodic event example");

    // Initialize the periodic timer
    periodic_msg.content.ptr = NULL;
    periodic_msg.sender_pid = thread_getpid();
    periodic_msg.type = 0;

    evtimer_init_msg(&periodic_timer);

    // Add the periodic event
    evtimer_add_msg(&periodic_timer, &periodic_msg, xtimer_ticks32(PERIODIC_INTERVAL), periodic_callback, NULL);

    // Main loop to keep the thread alive
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0) {
            periodic_callback(NULL);
        }
    }

    return 0;
}
