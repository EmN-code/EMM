#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "ztimer.h"
#include "stdio.h"  // Moved up in order according to the constraints
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define INTERVAL (1U * US_PER_SEC)  // Define 1-second interval using microsecond conversion

void event_callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered.");
}

int main(void) {
    evtimer_msg_t timer;
    evtimer_msg_init_queue(&timer);

    evtimer_msg_add_periodic(&timer, THREAD_PRIORITY_MAIN - 1, INTERVAL, event_callback, NULL);

    while (1) {
        pause();  // Let the system process events
    }

    return 0;
}