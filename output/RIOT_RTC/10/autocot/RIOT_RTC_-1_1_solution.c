#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t callback = alarm_callback;

    // Set current date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set alarm time
    time.tm_sec += 5; // Add 5 seconds for the alarm

    if (rtc_set_alarm(&time, callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}