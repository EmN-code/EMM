
#include <stdio.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

// Define a callback function for the periodic event
void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    msg_t msg;
    msg.content.ptr = NULL;
    msg.type = 0;

    // Add the periodic event to the event timer
    evtimer_add_msg(&evtimer, &msg, event_periodic_callback, NULL, ZTIMER_SEC, 1 * US_PER_SEC);

    // Start the periodic event
    evtimer_start(&evtimer);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
