#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "thread.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_alarm_cb_t cb = alarm_callback;

    /* Set the date to June 1, 2024, 00:00:00 */
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is 0-indexed
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    /* Initialize the RTC */
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    /* Set RTC time */
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    /* Set an alarm for 5 seconds from now */
    timeinfo.tm_sec += 5;
    if (rtc_set_alarm(&timeinfo, cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    /* Wait for the alarm to trigger */
    while (1) {
        thread_yield(); // Yield to allow other processes (like the RTC) to run
    }

    return 0;
}