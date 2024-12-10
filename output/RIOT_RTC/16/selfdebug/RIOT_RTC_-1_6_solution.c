#include <xtimer.h> // For xtimer_sleep
#include <stdio.h> // For printf
#include <time.h> // For struct tm and time functions
#include <periph/rtc.h> // For rtc functions

void rtc_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,  // Zero-based month, so 5 is June
        .tm_mday = 1, // Day of the month
        .tm_hour = 0, // Midnight
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set the RTC time to June 1, 2024
    rtc_set_time(&time);

    // Set the alarm for 5 seconds from now
    xtimer_sleep(5);
    rtc_set_alarm(rtc_callback, NULL, &time);

    while (1) {
        // Main loop does nothing, waiting for the alarm interrupt
    }
    return 0;
}