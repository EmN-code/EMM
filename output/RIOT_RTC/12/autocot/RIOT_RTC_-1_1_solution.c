#include <stdio.h>
#include "rtc.h"
#include "msg.h"

// Callback function for alarm
void alarm_cb(void *arg) {
    (void)arg; // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set up an alarm for 5 seconds from now
    struct tm alarm_time;
    alarm_time = time; // Copy the current time
    alarm_time.tm_sec += 5; // Add 5 seconds to current time

    // Set the alarm
    rtc_set_alarm(&alarm_time, alarm_cb, NULL);

    while (1) {
        // Main loop does nothing, waiting for alarm to trigger
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}