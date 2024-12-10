#include <stdio.h>
#include <sys/xtimer.h>
#include <periph/rtc.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused variable
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // Month 0-11 -> June
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&time);

    // Set the RTC alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    printf("RTC Alarm set for 5 seconds from June 1st, 2024.\n");

    // Entering a low-power mode if possible or loop indefinitely
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}