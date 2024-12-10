// Description: This code snippet sets up an RTC on an ESP32 board using RIOT OS. It initializes the RTC, sets the date to June 1, 2024, and sets an alarm to trigger after 5 seconds. When the alarm is triggered, a message "Alarm triggered!" is printed.

// Imports for setting up RTC and printing output
#include <stdio.h>  // For printf function
#include "xtimer.h"  // For xtimer_sleep function, if needed for delay or additional timing
#include "periph/rtc.h"  // Includes RTC peripheral access

// Alarm callback function declaration
void alarm_callback(void *arg) {
    (void)arg; // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    struct tm time;

    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = january
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time to June 1, 2024, 00:00:00
    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set an alarm to trigger after 5 seconds
    xtimer_sleep(5);
    
    if (rtc_set_alarm(NULL, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set. Waiting for alarm to trigger...\n");

    // Since this is an embedded environment, we'll simulate a wait
    while (1) {
        xtimer_sleep(1);  // Wait in a low-power state
    }

    return 0;
}