
#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

#define ALARM_DELAY_SEC (5U)

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Months since January (0-11)
    time.tm_mday = 1;           // Day of the month (1-31)
    time.tm_hour = 0;           // Hours since midnight (0-23)
    time.tm_min = 0;            // Minutes after the hour (0-59)
    time.tm_sec = 0;            // Seconds after the minute (0-59)
    time.tm_isdst = -1;         // Daylight Saving Time flag

    rtc_set_time(&time);

    xtimer_sleep(ALARM_DELAY_SEC);

    rtc_set_alarm(&time, ALARM_DELAY_SEC, alarm_callback, NULL);

    return 0;
}