#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>

#define INTERVAL (ZTIMER_MSEC_BASE_FREQ / 1000)  // 1 second interval

static ztimer_t timer;

void timer_callback(void *arg) {
    puts("Timer callback has been triggered!");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);  // Reschedule the timer
}

int main(void) {
    printf("Starting timer demo on RIOT for ESP32\n");
    
    // Initialize the timer
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    timer.callback = timer_callback;

    // Let the timer run
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);  // Just keep the CPU idle
    }

    return 0;
}