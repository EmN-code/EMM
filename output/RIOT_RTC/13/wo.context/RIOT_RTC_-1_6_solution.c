#include <xtimer.h>
#include <stdio.h>
#include "periph/rtc.h"

void rtc_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);

    /* Set an alarm to trigger after 5 seconds from now */
    xtimer_sleep(5);  // Correctly declare xtimer_sleep here

    rtc_t alarm;
    rtc_get_time(&alarm);
    alarm.tm_sec += 5;

    if (rtc_set_alarm(&alarm, rtc_callback, NULL) < 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    /* Keep the program running to wait for the alarm */
    while (1) {}

    return 0;
}