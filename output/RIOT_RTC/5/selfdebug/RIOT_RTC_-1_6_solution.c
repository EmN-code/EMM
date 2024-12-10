#include <stdio.h>
#include <sys/types.h>  // Make sure relevant headers are included

// Correct function declaration with parameter type
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_t rtc;
    rtc_alarm_t alarm;
    rtc_time_t time;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // 0-based month
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Assume rtc_set_time is defined elsewhere
    rtc_set_time(&rtc, &time);

    // Set alarm 5 seconds from the set time
    alarm.time = time;
    alarm.time.tm_sec += 5;  // Add 5 seconds
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    // Assume rtc_set_alarm is defined elsewhere
    rtc_set_alarm(&rtc, &alarm);

    // Main loop
    while (1) {
        // Code to keep main running, e.g., event loop, etc.
    }

    return 0;
}