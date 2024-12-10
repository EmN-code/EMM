#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_get_time(&time); // Get the current time
    printf("Current time: %04i-%02i-%02i %02i:%02i:%02i\n", 
           time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
           time.tm_hour, time.tm_min, time.tm_sec);

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm for 5 seconds after the set date
    time.tm_sec += 5;
    mktime(&time); // Normalize the time structure
    rtc_set_alarm(&time, rtc_alarm_cb, NULL);

    printf("Alarm set for: %04i-%02i-%02i %02i:%02i:%02i\n", 
           time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
           time.tm_hour, time.tm_min, time.tm_sec);

    // Wait for the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}