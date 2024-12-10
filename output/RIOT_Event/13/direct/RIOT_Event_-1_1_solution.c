#include <xtimer.h>
#include <stdio.h>

#define INTERVAL (1000000U)  // 1 second in microseconds

// Callback function for the periodic event
void event_periodic(void)
{
    printf("Periodic event triggered!\n");
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    printf("Starting event_periodic every 1 second...\n");

    while (1) {
        // Trigger the periodic event
        event_periodic();

        // Wait for the next interval
        last_wakeup = xtimer_milliseconds_until_next_alarm(last_wakeup, INTERVAL);
        xtimer_sleep_until(&last_wakeup);
    }

    return 0;  // Never reached
}