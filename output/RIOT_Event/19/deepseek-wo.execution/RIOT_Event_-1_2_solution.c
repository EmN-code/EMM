
#include <stdio.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t periodic_timer;
static evtimer_msg_event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic timer
    evtimer_init_msg(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Set the periodic timer to trigger every second
    evtimer_add_msg(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
