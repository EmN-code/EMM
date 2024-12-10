// Description of the problem and approach:
// This code is designed to configure and utilize the Real-Time Clock (RTC) on an ESP32 using RIOT OS. The goal is to set the date to June 1, 2024, and trigger an alarm 5 seconds later, with a message "Alarm triggered!" printed when the alarm activates. The issue encountered was a missing 'rtc.h' file. To resolve this, it's essential to ensure that the necessary header files are included in the correct order as provided in the initial setup, and setup the RTC alarm correctly.

// Necessary includes for the RIOT project
#include <stdio.h>
#include <stdint.h>
#include <xtimer.h>
#include <ztimer.h>
#include <periph/rtc.h>

// Callback function for the RTC alarm
void alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Setup date for June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month, where 0 = jan
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the time to the RTC
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm for 5 seconds after current time
    ztimer_sleep(ZTIMER_MSEC, 5000);

    // Set the alarm
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}