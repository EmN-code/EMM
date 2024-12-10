#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Create RTC time structure for June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // tm_year is year since 1900
    time.tm_mon = 6 - 1;         // tm_mon is month from 0 to 11
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set alarm for 5 seconds ahead
    rtc_get_time(&time);
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Keep main thread running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}