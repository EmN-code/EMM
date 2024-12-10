#include <xtimer.h>  // Ensure this is added for timer functionality
#include <stdio.h>
#include <periph/rtc.h>  // Include RTC peripheral functionality

void rtc_alarm_cb(void *arg) {
    (void)arg;  // To avoid unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 6 - 1,         // Month, where 0 = January
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    // Set an alarm for 5 seconds in the future
    xtimer_sleep(5);

    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm.\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now.\n");
    while (1) {
        // Put CPU to sleep, waiting for the alarm
        xtimer_sleep(10);
    }
    return 0;
}