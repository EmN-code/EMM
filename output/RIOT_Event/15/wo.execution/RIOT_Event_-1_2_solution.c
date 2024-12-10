#include "thread.h"
#include "event/thread.h"
#include "ztimer.h"
#include <stdio.h>

void timer_callback(void *arg) {
    printf("Callback function triggered.\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_periodic_init(ZTIMER_MSEC, &timer, 1000); // Setup a periodic timer with 1 second (1000 milliseconds) interval
    
    // Event loop
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}