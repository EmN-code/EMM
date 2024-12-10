#include <stdio.h>
#include "xtimer.h"  // For delaying purposes
#include "periph/rtc.h"  // Ensure the RTC peripheral library is included

void callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 6 - 1,    // Months since January
        .tm_mday = 1,      // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    if (rtc_init() != 0) {
        puts("Error: RTC initialization failed");
        return 1;
    }

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        puts("Error: setting RTC time failed");
        return 1;
    }

    // Set an alarm for 5 seconds from now
    rtc_alarm_t alarm;
    alarm.time = time;
    alarm.time.tm_sec += 5;
    alarm.callback = callback;

    if (rtc_set_alarm(&alarm) != 0) {
        puts("Error: setting RTC alarm failed");
        return 1;
    }

    puts("Alarm set for 5 seconds from now...");

    // Enter an infinite loop doing nothing, the callback will handle the alarm
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}