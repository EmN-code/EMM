# The solution involves setting up the Real-Time Clock (RTC) to trigger an alarm event. 
# We will initiate the RTC, set the current date and time, configure an alarm, and 
# define a callback function that gets called when the alarm is triggered.

#include "thread.h"
#include "rtc.h"
#include <stdio.h>

#define ALARM_SECONDS 5

// Callback function for the RTC alarm
void alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    struct tm alarmspec;

    // Assuming rtc_init() initializes the RTC hardware
    rtc_init();

    // Set the RTC date and time to June 1, 2024, at 00:00:00
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1;        // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Configure the alarm to trigger after ALARM_SECONDS (5 seconds from now)
    rtc_get_time(&time); // Get the current RTC time

    alarmspec = *gmtime(&time.tv_sec);
    alarmspec.tm_sec += ALARM_SECONDS;

    rtc_set_alarm(&alarmspec, alarm_callback, NULL);

    // Main loop does nothing, just waits for events
    while (1) {
        thread_yield();
    }
    return 0;
}