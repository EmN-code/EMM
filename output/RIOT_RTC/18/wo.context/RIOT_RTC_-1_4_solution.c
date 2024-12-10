#include <stdio.h>
#include <time.h>
#include <thread.h>
#include <stdbool.h>
#include <sched.h>
#include <mutex.h>
#include <rtc.h>

#define RTC_OK 0

// Callback function for the alarm
void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm set_time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 6 - 1,       // Month 0 - Jan
        .tm_mday = 1,          // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Convert to time_t format
    time_t epoch_time = mktime(&set_time);

    // Set RTC time
    if (rtc_set_time(epoch_time) != RTC_OK) {
        printf("Failed to set RTC time!\n");
        return -1;
    }

    // Set alarm 5 seconds later
    time_t alarm_time = epoch_time + 5;

    rtc_alarm_cb_t alarm;
    alarm.time = alarm_time;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    // Set RTC alarm
    if (rtc_set_alarm(&alarm) != RTC_OK) {
        printf("Failed to set RTC alarm!\n");
        return -1;
    }

    puts("RTC alarm set. Waiting for the alarm to trigger.");

    // Busy loop to wait for the alarm
    while (true) {
        thread_sleep();
    }

    return 0;
}