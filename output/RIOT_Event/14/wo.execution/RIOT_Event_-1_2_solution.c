#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "event/callback.h"

void _periodic_event_callback(void *arg)
{
    (void)arg;
    // Here, you would handle the periodic event
}

int main(void)
{
    evtimer_msg_t evtimer;
    event_t event;
    
    evtimer_init_msg(&evtimer);
    
    event.callback = _periodic_event_callback;
    event.event.data = &evtimer;
    
    while(1) {
        xtimer_usleep(1000000); // Sleep for 1 second
        // Trigger the event callback
        event.callback(event.event.data);
    }
    return 0;
}