#include <ztimer.h>
#include <stdio.h>

void event_periodic(void *arg) {
    printf("Callback function triggered\n");
}

int main(void) {
    printf("Initializing event timer\n");
    ztimer_t timer;
    ztimer_init(ZTIMER_USEC, &timer, ZTIMER_MSEC_TO_TICKS(1000), event_periodic, NULL);
    ztimer_set(ZTIMER_USEC, &timer, ZTIMER_MSEC_TO_TICKS(1000));
    
    return 0;
}