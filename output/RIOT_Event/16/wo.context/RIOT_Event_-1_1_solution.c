#include "xtimer.h"
#include "stdio.h"
#include "event/callback.h"
#include "event/periodic.h"

void periodic_callback(void *arg)
{
    (void)arg; // Cast to void to avoid unused parameter warning
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    printf("RIOT Callback Example on ESP32\n");
    
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);

    evtimer_periodic_init_event(&event, &periodic_callback, NULL, 1000000);

    evtimer_add_msg(&evtimer, &event, NULL);
    
    while (1) {
        evtimer_dispatch(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}