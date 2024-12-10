#include <xtimer.h>
#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    
    /* Set date to June 1, 2024 */
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;  // June is month 5 when starting from 0
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    /* Initialize the RTC */
    rtc_init();

    /* Set the RTC time */
    rtc_set_time(&time);

    /* Set an alarm for 5 seconds later */
    time.tm_sec += 5;
    rtc_set_alarm(&time, rtc_alarm_cb, NULL);

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}