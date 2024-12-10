#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)  // interval of 1 second

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    printf("Callback function executed every second.\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
}

int main(void) {
    printf("Starting periodic event example with RIOT on ESP32\n");

    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    return 0;
}