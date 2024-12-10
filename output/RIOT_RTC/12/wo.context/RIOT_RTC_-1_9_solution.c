#include <xtimer.h>
#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    xtimer_ticks32_t interval = xtimer_usec_to_ticks(5000000); // 5 seconds
    
    // Set an xtimer that executes alarm_callback after interval and stays idle.
    xtimer_t timer;
    timer.callback = alarm_callback;
    timer.arg = NULL;

    printf("Setting alarm for 5 seconds from now...\n");

    // Arm timer
    xtimer_set(&timer, interval);

    return 0;
}