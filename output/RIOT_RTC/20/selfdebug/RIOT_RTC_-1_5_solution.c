#include <xtimer.h>
#include <stdio.h>
#include <periph/rtc.h>

void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // Month, where 0 = January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    struct tm alarm_time = time;
    xtimer_sleep(5); // wait for 5 seconds
    rtc_get_time(&time); // updating time
    alarm_time.tm_sec = time.tm_sec + 5;

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    while (1) {
        // Sleep for a while to allow the alarm to trigger
        xtimer_sleep(1);
    }

    return 0;
}