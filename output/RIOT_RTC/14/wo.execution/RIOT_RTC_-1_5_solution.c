#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <rtc/periph.h>
#include <arpa/inet.h>
#include <gpio/periph.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <netinet/in.h>
#include <kernel_defines.h>

#define ALARM_DURATION (5U)

static void alarm_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    rtc_set_time_t rtc_time;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 5; // Month, 0 - January
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_time.time = mktime(&timeinfo);

    if (rtc_set_time(&rtc_time) != 0) {
        printf("Failed to set time\n");
        return -1;
    }

    // Set alarm
    rtc_alarm_t alarm;
    alarm.time = rtc_time.time + ALARM_DURATION;
    alarm.callback = alarm_callback;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    // Let the system wait to trigger the alarm
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}