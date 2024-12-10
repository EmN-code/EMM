#include <stdio.h>
#include <rtc.h>
#include <event.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t current_time;
    current_time.tm_year = 2024;
    current_time.tm_mon = 5; // June, zero-based index
    current_time.tm_mday = 1;
    current_time.tm_hour = 0;
    current_time.tm_min = 0;
    current_time.tm_sec = 0;

    if (rtc_set_time(&current_time) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    rtc_alarm_t alarm;
    alarm.time.tm_sec = 5;  // Set alarm 5 seconds later
    alarm.callback = rtc_alarm_callback;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm.\n");
        return 1;
    }

    // Enter idle mode
    while (1) {
        // Main loop can perform other tasks or just idle
    }

    return 0;
}