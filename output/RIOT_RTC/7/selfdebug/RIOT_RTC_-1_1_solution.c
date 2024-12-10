#include <stdio.h>
#include <rtc/rtc.h>
#include <msg.h>

// Callback function for RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,            // Months since January [0-11]
        .tm_mday = 1,           // Day of the month [1-31]
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Error: Could not set RTC time\n");
        return 1;
    }

    // Setup and set an alarm for 5 seconds later
    time.tm_sec += 5; // Adding 5 seconds
    if (rtc_set_alarm(&time, rtc_alarm_callback, NULL) != 0) {
        printf("Error: Could not set RTC alarm\n");
        return 1;
    }

    // Wait indefinitely for the alarm to trigger
    msg_t msg;
    msg_receive(&msg);

    return 0;
}