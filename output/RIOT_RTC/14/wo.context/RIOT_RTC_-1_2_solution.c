#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

// Callback function for RTC alarm
void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5; // Months since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize and set the RTC time
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return -1;
    }

    // Set alarm 5 seconds from the current time
    alarm.time = time;
    alarm.time.tm_sec += 5;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    printf("RTC initialized and alarm set for 5 seconds from now\n");

    // Simulate a low-power mode or wait indefinitely
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}