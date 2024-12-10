#include <stdio.h>
#include <string.h>
#include "ztimer.h"
#include "xtimer.h"
#include "rtc.h"

void setup_rtc_and_alarm(void) {
    struct tm timeinfo;
    rtc_time_t alarm_time;
    rtc_get_time_t current_time;

    /* Set the RTC time to June 1, 2024, 00:00:00 */
    memset(&timeinfo, 0, sizeof(struct tm));
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 5;           // tm_mon is 0-indexed, June is month 5
    timeinfo.tm_mday = 1;

    rtc_set_time(&timeinfo);

    /* Get the current time */
    rtc_get_time(&current_time);

    /* Set the alarm to trigger 5 seconds from now */
    alarm_time = current_time + 5;
    rtc_set_alarm(&alarm_time);
}

void rtc_alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm Example\n");
    setup_rtc_and_alarm();

    /* Register the alarm handler */
    rtc_set_alarm_callback(rtc_alarm_handler);

    while (1) {
        /* The main loop can do other tasks, or just wait for alarm */
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}