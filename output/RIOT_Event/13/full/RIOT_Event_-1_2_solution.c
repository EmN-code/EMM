#include "ztimer.h"
#include "xtimer.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/periodic.h"
#include "event/thread.h"
#include "event/callback.h"

void callback_function(void *arg) {
    printf("Event triggered every second.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    evtimer_set_periodic(&evtimer, ztimer_now(ZTIMER_MSEC), 1000, &event.super);
    
    while(1) {
        evtimer_next_event(&evtimer);
    }

    return 0;
}