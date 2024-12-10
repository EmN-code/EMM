#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include <stdio.h>

// Callback function for RTC alarm
void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    int rc;

    // Initialize RTC time to June 1, 2024
    // Ensure that the time is appropriately set
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time with retry logic
    while (1) {
        rc = rtc_set_time(&time);
        if (rc != 0) {
            printf("Failed to set RTC time. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC time set successfully.\n");
            break;
        }
    }

    // Set alarm for 5 seconds later
    alarm_time = time;
    alarm_time.tm_sec += 5;

    // Set the RTC alarm with retry logic
    while (1) {
        rc = rtc_set_alarm(&alarm_time, alarm_cb, NULL);
        if (rc != 0) {
            printf("Failed to set RTC alarm. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC alarm set successfully.\n");
            break;
        }
    }

    // Main loop
    while (1) {
        // Keep running, RTC alarm will trigger the callback
        xtimer_sleep(10);
    }
    return 0;
}