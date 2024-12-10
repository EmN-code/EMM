
#include <stdio.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t periodic_timer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    puts("Starting periodic event timer example");

    // Initialize the event timer
    evtimer_init_msg(&periodic_timer);

    // Create a periodic event
    evtimer_msg_event_t periodic_event;
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Set the periodic event
    evtimer_add_msg(&periodic_timer, &periodic_event, sched_active_pid, PERIODIC_INTERVAL);

    // Start the periodic event
    evtimer_add_msg(&periodic_timer, &periodic_event, sched_active_pid, PERIODIC_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
