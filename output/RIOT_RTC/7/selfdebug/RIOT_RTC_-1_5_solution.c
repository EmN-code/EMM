#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <timex.h>

#define ALARM_TIME_SEC 5

void alarm_callback(void *arg) {
    (void)arg; // Mark unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize and set RTC time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // tm_year is the year since 1900
    timeinfo.tm_mon = 5;            // June (months since January, 0-11)
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);

    // Calculate future time for alarm (5 seconds from now)
    ztimer_now_t timeout = ztimer_now(ZTIMER_MSEC) + ALARM_TIME_SEC * MS_PER_SEC;
    ztimer_alarm_t alarm;
    ztimer_alarm_init(&alarm, ZTIMER_MSEC, alarm_callback, NULL);

    // Set the alarm
    ztimer_alarm(&alarm, timeout);

    // Main loop
    while (1) {
        // Normally you would put the CPU to sleep to save power here
    }

    return 0;
}