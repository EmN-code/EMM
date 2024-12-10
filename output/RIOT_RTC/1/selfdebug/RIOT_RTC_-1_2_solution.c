#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"
#include "periph_cpu.h"

// Callback function to handle the RTC alarm
static void alarm_cb(void) {
    printf("Alarm triggered!\n");
}

// Main function to set the date and configure the alarm
int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = jan
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize the RTC with the current time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set the alarm to trigger 5 seconds from the set time
    alarm_time = time;
    alarm_time.tm_sec += 5;

    // Configure the RTC alarm
    if (rtc_alarm_set(&alarm_time, alarm_cb) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Run indefinitely
    while (1) {
        // Sleep to save power, as the RTC will interrupt
        xtimer_sleep(10);  // Sleep in 10 second intervals
    }

    return 0;
}