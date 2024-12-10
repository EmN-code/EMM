#include <ztimer.h>
#include <stdio.h>

void event_periodic(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, 1000, event_periodic, NULL);
    
    while (1) {
        // Here a delay could be included if needed
    }
    return 0;
}