#include "xtimer.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "stdbool.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "timex.h"
#include "mutex.h"

#define TIMER_INTERVAL (1000000U) // Timer interval in microseconds

void timer_callback(void *arg) {
    (void)arg; // Ignoring unused parameter
    printf("Timer callback triggered!\n");
    // Here we should turn on the LED (omitted)
}

int main(void) {
    ztimer_clock_t *clock = ztimer_clock_get(ZTIMER_MSEC);
    ztimer_ticks32_t interval = ZTIMER_MSEC_TO_TICKS(1000);

    while (true) {
        if (ztimer_set_wakeup(clock, ZTIMER_MSEC, interval, timer_callback, NULL) != 0) {
            printf("Error setting timer\n");
            continue;
        }
        break;
    }

    // Main loop
    while (1) {
        // Ideally, we would perform other tasks here
        ztimer_sleep(ztimer_clock_get(ZTIMER_MSEC), interval);
    }

    return 0;
}