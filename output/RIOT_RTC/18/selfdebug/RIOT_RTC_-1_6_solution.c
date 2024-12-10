#include "xtimer.h"
#include "periph/rtc.h"
#include <stdio.h>

static void rtc_alarm(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_set_time(2024, 6, 1, 0, 0, 0);
    printf("RTC time set to June 1, 2024\n");

    rtc_t alarm;
    alarm.tm_year = 2024 - RTC_BASEYEAR;
    alarm.tm_mon = 6 - 1;
    alarm.tm_mday = 1;
    alarm.tm_hour = 0;
    alarm.tm_min = 0;
    alarm.tm_sec = 5;

    rtc_set_alarm(&alarm, rtc_alarm, NULL);
    printf("Alarm set for 5 seconds later\n");
    
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}