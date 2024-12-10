#include <stdio.h>  // For printf
#include "xtimer.h"  // For xtimer_usleep
#include "periph/rtc.h"  // For RTC functionalities

// Callback function for the RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg;  // just to suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set the date and time - June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month [0, 11]
    time.tm_mday = 1;            // Day of the month [1, 31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm after 5 seconds from now
    xtimer_usleep(5 * US_PER_SEC); // Wait for 5 seconds

    // Set the alarm callback
    rtc_set_alarm(NULL, rtc_alarm_callback, NULL);

    // Loop indefinitely
    while (1) {
        // Sleep to save power while waiting for the alarm
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}