#include <timex.h>
#include <stdio.h>
#include <rtc.h>
#include <event/periodic.h>

int main(void) {
    rtc_time_t rtc_time;
    rtc_time.year = 2024 - 1900; // RTC year is usually calculated as the year offset from 1900
    rtc_time.month = 6;         // June
    rtc_time.day = 1;
    rtc_time.hour = 0;
    rtc_time.minute = 0;
    rtc_time.second = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time(&rtc_time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm for 5 seconds after the current time
    rtc_alarm_t alarm;
    alarm.time = rtc_time;
    alarm.time.second += 5; // Add 5 seconds
    alarm.cb = my_alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC configured. Alarm set for 5 seconds later.\n");
    return 0;
}

// Callback function for alarm
void my_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}