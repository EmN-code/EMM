#include "ztimer.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "unistd.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "stdio.h"
#include "arpa/inet.h"

#include "kernel_defines.h"

#define TIMER_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    ztimer_t timer;
    evtimer_t evtimer;
    evtimer_event_t event;
    uint32_t now;

    // Initialize the timer
    timer.callback = alarm_callback;
    timer.arg = NULL;

    // Get the current time
    now = ztimer_now(ZTIMER_MSEC);

    // Set an event for 5 seconds from now
    evtimer_add_mbox(&evtimer, &event, now + TIMER_INTERVAL);

    // Main loop
    while(1) {
        // Handling exception or additional logic if needed
        xtimer_sleep(1);
        printf("Waiting for alarm...\n");
    }
    return 0;
}