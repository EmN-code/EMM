#include <xtimer.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)  // Set the interval for 1 second

void callback_function(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Callback function triggered\n");
}

int main(void) {
    
    xtimer_ticks32_t interval = xtimer_ticks32_t{.ticks32 = INTERVAL};
    xtimer_ticks32_t last_wakeup = xtimer_now();

    printf("Starting periodic event every second\n");
    
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, interval);
        callback_function(NULL);
    }

    return 0;
}