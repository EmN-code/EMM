#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

int main(void) {
    // Initialize RTC
    rtc_init();

    // Define and set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1;        // tm_mon is months since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Print current date to confirm
    printf("RTC set to: %04d-%02d-%02d %02d:%02d:%02d\n",
           time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
           time.tm_hour, time.tm_min, time.tm_sec);

    // Define alarm time (5 seconds after current time)
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Normalize the time structure to account for overflow
    mktime(&alarm_time);

    // Set the alarm
    rtc_set_alarm(&alarm_time, NULL, NULL);

    // Wait for alarm
    while (1) {
        if (rtc_alarm_pending()) {
            printf("Alarm triggered!\n");
            rtc_clear_alarm();
            break;
        }
        xtimer_sleep(1);
    }

    return 0;
}