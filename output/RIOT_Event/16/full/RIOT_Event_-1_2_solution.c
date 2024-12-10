#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "ztimer.h"
#include <stdio.h>

#define TIMER_INTERVAL_SEC 1  // Timer interval of 1 second

static void timer_callback(void *arg) {
    puts("Timer callback triggered!");
}

int main(void) {
    ztimer_clock_t clock = ZTIMER_SEC;
    ztimer_callback_t timer;

    ztimer_set_msg(&clock, &timer, timer_callback, NULL, TIMER_INTERVAL_SEC * MS_PER_SEC);
    
    while (1) {
        // Main loop can perform other tasks
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    
    return 0;
}
