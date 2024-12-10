#include "xtimer.h"
#include "timex.h"
#include "stdio.h"

void event_periodic(void *arg) {
    (void)arg; // To avoid unused parameter warning
    printf("Periodic event triggered\n");
}

int main(void) {
    /* Initialize timer */
    static xtimer_t timer;
    xtimer_set_callback(&timer, event_periodic, NULL);
    
    /* Start the timer to trigger every second */
    while (1) {
        xtimer_set(&timer, 1 * US_PER_SEC); // Set timer interval
        xtimer_sleep(1); // Wait for a second
    }

    return 0;
}