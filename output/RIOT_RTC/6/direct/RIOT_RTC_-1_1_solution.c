#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm example on ESP32\n");

    struct tm time;
    rtc_get_time(&time);

    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5; // months since January [0-11]
    time.tm_mday = 1; // day of the month [1-31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    printf("Current time set to: %d-%02d-%02d %02d:%02d:%02d\n",
           1900 + time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

    xtimer_sleep(5); // simulate delay before setting the alarm

    rtc_get_time(&time);
    time.tm_sec += 5; // set alarm to 5 seconds from now

    if (rtc_set_alarm(&time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds later\n");

    while (1) {
        // Idle loop
    }

    return 0;
}