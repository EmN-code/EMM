// Description: This code sets up an RTC alarm on an ESP32 using the RIOT operating system. It initializes
// the RTC, sets the current date, and configures an alarm to trigger after 5 seconds, displaying "Alarm triggered!".

#include "xtimer.h"
#include "periph/rtc.h"
#include <stdio.h> // Required for printf to work

// Alarm callback function
void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Define a tm struct and set the date to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // Month 0-11 (June)
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm 5 seconds from now
    time.tm_sec += 5; // add 5 seconds
    if (rtc_set_alarm(&time, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set. Waiting for alarm to trigger...\n");

    // Enter an idle loop to wait for the alarm
    while (1) {
        xtimer_sleep(1); // Sleep to reduce CPU usage
    }

    return 0;
}