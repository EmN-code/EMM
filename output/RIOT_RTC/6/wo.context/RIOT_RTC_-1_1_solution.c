#include <stdio.h>
#include <rtc.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900;  // year since 1900
    time.tm_mon = 6 - 1;  // months since January
    time.tm_mday = 1;  // day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    rtc_init();
    rtc_set_time(&time);

    // Set alarm for 5 seconds from now
    time.tm_sec += 5;
    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    while (1) {
        // Loop forever
    }

    return 0;
}