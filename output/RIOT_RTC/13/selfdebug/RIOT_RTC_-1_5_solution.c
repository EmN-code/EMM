#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

// Alarm callback function
void rtc_alarm_cb(void *arg) {
    (void)arg; // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // 0-based (May)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Setup the alarm 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    // Suspend main thread (RTC and callbacks handle the rest)
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}