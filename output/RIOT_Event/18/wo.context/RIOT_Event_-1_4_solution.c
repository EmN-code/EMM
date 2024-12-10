#include <ztimer.h>
#include <stdio.h>

ztimer_t timer;

void event_periodic(void *arg) {
    (void)arg;
    printf("Event triggered\n");
    ztimer_set(ZTIMER_MSEC, &timer, 1000);
}

int main(void) {
    ztimer_init();  // Ensure the ztimer module is initialized
    ztimer_set(ZTIMER_MSEC, &timer, 1000);
    timer.callback = event_periodic;
    timer.arg = NULL;
    printf("Timer started\n");
    while (1) {
        // Main loop can contain additional logic
    }
    return 0;
}