#include <stdio.h>
#include <string.h>
#include "timex.h"
#include "ztimer.h"

void set_rtc(time_t *rtc_time) {
    struct tm *timeinfo = gmtime(rtc_time);
    /* Set rtc_time to June 1, 2024 */
    timeinfo->tm_year = 2024 - 1900; // years since 1900
    timeinfo->tm_mon = 5;  // months since January
    timeinfo->tm_mday = 1; // day of the month
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    *rtc_time = mktime(timeinfo);
}

void rtc_alarm_function(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC alarm example\n");
    
    time_t rtc_time;
    set_rtc(&rtc_time);

    timex_t now;
    ztimer_now(ZTIMER_SEC, &now);

    uint32_t sleep_time = 5; // sleep time 5 seconds
    ztimer_sleep(ZTIMER_SEC, sleep_time);

    rtc_alarm_function(NULL);

    return 0;
}