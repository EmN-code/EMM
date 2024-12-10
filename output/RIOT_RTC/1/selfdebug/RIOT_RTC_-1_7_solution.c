#include <stdio.h>
#include <stdint.h>

// Assuming necessary RTC and alarm headers here
// #include "rtc.h"

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the initial date and time: June 1, 2024, at 00:00:00
    rtc_time_t rtc_time;
    rtc_time.tm_year = 2024;
    rtc_time.tm_mon = 6;
    rtc_time.tm_mday = 1;
    rtc_time.tm_hour = 0;
    rtc_time.tm_min = 0;
    rtc_time.tm_sec = 0;

    // Initialize RTC with the specified time
    rtc_init();
    rtc_set_time(&rtc_time);

    // Configure the alarm to trigger after 5 seconds
    rtc_alarm_t rtc_alarm;
    rtc_get_time(&rtc_time); // Make sure we're getting current time into the structure
    rtc_alarm.time = rtc_time;
    rtc_alarm.time.tm_sec += 5;  // Add 5 seconds for the alarm
    rtc_alarm.callback = alarm_callback;
    rtc_alarm.arg = NULL;

    // Set the alarm
    rtc_set_alarm(&rtc_alarm);

    // Waiting mechanism to keep the main function running
    while (1) {
        // In an actual implementation, put the MCU in a low-power sleep mode
    }

    return 0;
}