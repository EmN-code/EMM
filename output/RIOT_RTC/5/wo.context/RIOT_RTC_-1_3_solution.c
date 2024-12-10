/* Solution code for setting an RTC alarm on ESP32 with RIOT OS */

// Importing headers related to logging, timers, and architecture/memory configuration.
#include <thread_config.h>
#include <stdio.h>
#include <cstring>
#include <time.h>

// Callback function for alarm
void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Initialize the time to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5;            // months since January [0, 11]
    time.tm_mday = 1;           // day of the month [1, 31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize Real Time Clock (RTC)
    if (rtc_init() != STATUS_OK) {
        printf("RTC initialization failed.\n");
        return -1;
    }

    // Set the date and time
    if (rtc_set_time(&time) != STATUS_OK) {
        printf("Setting RTC time failed.\n");
        return -1;
    }

    // Set up an alarm 5 seconds from now
    alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != STATUS_OK) {
        printf("Setting RTC alarm failed.\n");
        return -1;
    }

    printf("RTC alarm set for 5 seconds from now.\n");

    while (1) {
        // Simulate waiting indefinitely while the RTC handles its operation
    }

    return 0;
}
