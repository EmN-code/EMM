#include "ztimer.h"
#include "stdio.h"
#include "event.h"

#define INTERVAL (ZTIMER_MSEC_TO_USEC(1000)) // 1 second interval

static ztimer_t timer;

// Callback function
void timer_callback(void *arg) {
    puts("Timer tick");
}

int main(void) {
    // Initialize ztimer and set up a periodic timer
    ztimer_set(&timer, ZTIMER_MSEC, INTERVAL, timer_callback, NULL);

    while (1) {
        // Perform other tasks here or put the MCU to sleep
    }

    return 0;
}