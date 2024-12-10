#include <stdio.h>
#include "periph/rtc.h"
#include "periph/gpio.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time!\n");
        return 1;
    }

    // Set RTC alarm after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    if (rtc_set_alarm(&alarm_time, alarm_callback) != 0) {
        printf("Failed to set RTC alarm!\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    // Loops forever, relying on callback to handle the alarm
    while (1) {
        /* Just sleep and let interrupts do the work */
        xtimer_sleep(1);
    }
    return 0;
}