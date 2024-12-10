#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "msg.h"
#include "netinet/in.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "ztimer.h"
#include "xtimer.h"

#define ALARM_INTERVAL_SEC 5

void rtc_alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_time_t time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, where 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init() < 0) {
        puts("Failed to initialize RTC");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time(&time) < 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Set alarm to trigger after ALARM_INTERVAL_SEC seconds
    rtc_alarm_t alarm;
    memcpy(&alarm.time, &time, sizeof(rtc_time_t));
    alarm.time.tm_sec += ALARM_INTERVAL_SEC;
    mktime((struct tm *)&alarm.time); // Normalize the time structure

    if (rtc_set_alarm(&alarm, rtc_alarm_callback, NULL) < 0) {
        puts("Failed to set RTC alarm");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    // Put the main function to sleep (simulating device low power state)
    sleep(ALARM_INTERVAL_SEC + 1);

    return 0;
}
