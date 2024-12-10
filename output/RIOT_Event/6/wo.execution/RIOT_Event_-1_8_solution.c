#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "time_units.h"
#include "timex.h"
#include "evtimer_msg.h"

void callback(void *arg) {
    printf("Callback function executed\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t evt;

    // Initialize evtimer for handling events
    evtimer_init_msg(&evtimer);

    int period = (1 * 1000000); // 1 second in microseconds

    evt.event.callback = callback;
    evt.event.period = period;
    evt.event.offset = 0;

    printf("Starting the periodic timer...\n");
    while (1) {
        evtimer_add_msg(&evtimer, &evt, KERNEL_PID_UNDEF);
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}