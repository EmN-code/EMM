#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, toggling LED.\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Setting up timer to trigger every second...\n");

    while (1) {
        ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL);
        xtimer_sleep(1);
    }

    return 0;
}