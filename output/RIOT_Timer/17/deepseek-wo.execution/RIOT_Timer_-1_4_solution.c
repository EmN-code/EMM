
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void) {
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Main loop to keep the program running
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
