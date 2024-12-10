#include <stdio.h>
#include <time.h>
#include "periph_conf.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where 0 = January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    time_t epoch_time = mktime(&timeinfo);

    if (rtc_set_time(epoch_time) != 0) {
        printf("Failed to set time.\n");
        return -1;
    }

    rtc_ts_t alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time += 5; // Set alarm to trigger after 5 seconds

    rtc_alarm_t alarm = {
        .time = alarm_time,
        .callback = alarm_callback,
        .arg = NULL
    };

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm.\n");
        return -1;
    }

    while (1) {
        // Do nothing, waiting for alarm
    }

    return 0;
}
