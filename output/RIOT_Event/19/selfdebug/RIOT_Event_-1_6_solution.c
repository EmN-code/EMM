#include <xtimer.h>
#include <stdio.h>

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
    xtimer_periodic_wakeup(0, US_PER_SEC);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        event_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, US_PER_SEC);
    }
    
    return 0;
}