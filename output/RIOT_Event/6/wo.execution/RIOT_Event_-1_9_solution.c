#include "xtimer.h"
#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "string.h"
#include "timex.h"
#include "timex.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"

#define INTERVAL        (1U * US_PER_SEC)  // one second interval

kernel_pid_t main(void) {
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        puts("Event triggered");
    }

    return 0;
}

void timer_callback(void *arg) {
    (void)arg;
    printf("Callback fired!\n");
}