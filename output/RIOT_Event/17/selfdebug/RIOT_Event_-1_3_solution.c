#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (ZTIMER_SEC(1)) // 1 second interval

static ztimer_t timer;

void event_periodic(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL);
}

int main(void) {
    printf("Starting periodic timer\n");
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL);
    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL); // sleep until the next interval
        event_periodic(NULL);
    }
    return 0;
}