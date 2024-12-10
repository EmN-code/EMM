#include <stdio.h>
#include <periph/rtc.h>
#include <xtimer.h>
#include <thread.h>

// RTC callback function for the alarm
void rtc_alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    rtc_time_t rtc_time;

    // Set date to June 1, 2024, and time to 00:00:00
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon  = 6 - 1;       // tm_mon is month from 0 to 11
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min  = 0;
    timeinfo.tm_sec  = 0;

    rtc_time.time = mktime(&timeinfo);
    rtc_set_time(&rtc_time);

    // Simulation of alarm set to trigger after 5 seconds
    rtc_alarm_cb_t alarm;
    alarm.callback = rtc_alarm_callback;
    alarm.arg = NULL;
    rtc_time_t alarm_time;

    // Set alarm time as current time + 5 seconds
    rtc_get_time(&alarm_time);
    alarm_time.time += 5;

    rtc_set_alarm(&alarm, &alarm_time);

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}