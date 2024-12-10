#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL_SEC (1U)  // Timer interval (1 second)

static void timer_callback(void *arg) {
    (void)arg;  // Unused
    puts("Timer callback triggered");
}

int main(void) {
    ztimer_t periodic_timer;
    periodic_timer.callback = timer_callback;
    periodic_timer.arg = NULL;

    // Initialize timer
    ztimer_init(ZTIMER_MSEC);

    // Setup periodic event (corrected usage)
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, TIMER_INTERVAL_SEC * MS_PER_SEC);
    
    // Start periodic timer
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        sleep(TIMER_INTERVAL_SEC);
    }
    return 0;
}