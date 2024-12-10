#include "ztimer.h"
#include "msg.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "xtimer.h"
#include "event/thread.h"

#define INTERVAL (1U * US_PER_SEC)

static void cb_func(void *arg) {
    (void)arg;
    printf("Periodic callback every second\n");
}

int main(void) {
    ztimer_periodic_t timer;
    ztimer_init_periodic(ZTIMER_MSEC, &timer, cb_func, NULL, INTERVAL);
    ztimer_start_periodic(&timer);

    while (1) {
        ztimer_sleep(ZTIMER_SECOND, 1);
    }

    return 0;
}