#include "xtimer.h"
#include "periph/rtc.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    // Set date to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 5,  // 0 indexed months, so June is 5
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return -1;
    }

    // Set alarm 5 seconds from now
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    // Keep the main thread running to wait for the alarm callback
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}