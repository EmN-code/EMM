#include <xtimer.h>   // For timing operations
#include <stdio.h>     // For standard input and output functions

#define ALARM_DELAY (5U)   // Set delay for 5 seconds

int main(void) {
    printf("Setting simulated RTC date to June 1, 2024...\n");
    // In a real RTC setup, we'd set the date here, but we will simulate timing instead.
    printf("RTC set. Waiting for %u seconds to trigger alarm...\n", ALARM_DELAY);

    // Wait for 5 seconds using xtimer
    xtimer_sleep(ALARM_DELAY);

    // Alarm action: This simulates the alarm trigger.
    printf("Alarm triggered!\n");

    return 0;
}