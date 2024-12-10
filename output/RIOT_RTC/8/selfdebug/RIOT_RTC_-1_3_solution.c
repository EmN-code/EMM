#include <stdio.h>
#include <string.h>
#include <time.h>
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void) arg; // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_set_datetime(&(struct tm){
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    });

    /* Calculate alarm time, 5 seconds from set time */
    rtc_get_datetime(&timeinfo);
    time_t rawtime = mktime(&timeinfo);
    rawtime += 5; // 5 seconds later
    struct tm *alarm_time = gmtime(&rawtime);

    rtc_alarm_t alarm = {
        .time = *alarm_time,
        .callback = alarm_callback,
        .arg = NULL
    };

    rtc_set_alarm(&alarm);

    while (1) {
        // Busy wait or perform other tasks
    }

    return 0;
}