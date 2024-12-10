#include <stdio.h>
#include "ztimer/ztimer.h"

#define INTERVAL (ZTIMER_MSEC)  // Assuming the correct duration in milliseconds

ztimer_t my_timer;

void callback(void *arg) {
    (void)arg;  // Suppress unused argument warning
    printf("Callback function executed\n");
}

int main(void) {
    printf("Starting periodic timer example\n");

    ztimer_set_msg(&my_timer, ZTIMER_MSEC, INTERVAL, thread_getpid(), callback);
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);  // Sleep until next interval
        callback(NULL);  // Manually invoke the callback as a placeholder
    }

    return 0;
}