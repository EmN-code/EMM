#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <thread.h>
#include <time_units.h>

void periodic_callback(void *arg) {
    (void)arg; // To suppress unused parameter warning
    printf("Periodic callback executed every second.\n");
}

int main(void) {
    evtimer_t evtimer;  
    evtimer_event_t timer_event;

    evtimer_init(&evtimer, periodic_callback);

    // Set the timer event to trigger every 1 second (1,000,000 microseconds)
    xtimer_ticks32_t interval = xtimer_usec_from_sec(1);
    evtimer_add(&evtimer, &timer_event, interval);

    printf("Starting periodic callback every second.\n");
    while (1) {
        xtimer_sleep(1);
        // Callback will be called automatically
    }

    return 0;
}