#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

// This is the RTC alarm callback
void rtc_alarm_callback(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t callback = rtc_alarm_callback;
    int err;

    // Initialize the time structure to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    err = rtc_set_time(&time);
    if (err != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    
    // Create a loop to handle RTC initialization if it fails
    while (1) {
        err = rtc_set_alarm(rtc_time(), callback, NULL);
        if (err != 0) {
            printf("Trying to set RTC alarm again after failure.\n");
            xtimer_sleep(1);  // Wait for a second before retrying
        } else {
            printf("RTC alarm set successfully.\n");
            break;
        }
    }

    // Keep the main function running for the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}