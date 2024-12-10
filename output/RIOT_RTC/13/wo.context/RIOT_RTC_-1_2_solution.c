#include <stdio.h>
#include <time.h>
#include "periph_cpu.h"

int main(void) {
    // Initialize Real Time Clock (RTC) - assuming RTC is initialized in the periph_cpu.h file
    // Set the RTC date and time to June 1, 2024
    struct tm date_time;
    date_time.tm_year = 2024 - 1900; // Year since 1900
    date_time.tm_mon = 6 - 1;        // Month, where 0 = January
    date_time.tm_mday = 1;           // Day of the month
    date_time.tm_hour = 0;
    date_time.tm_min = 0;
    date_time.tm_sec = 0;

    // Convert to time_t format
    time_t target_time = mktime(&date_time);
    if (target_time == -1) {
        printf("Failed to set date and time.\n");
        return 1;
    }

    // Wait for 5 seconds to simulate alarm trigger
    sleep(5);

    // Trigger the alarm
    printf("Alarm triggered!\n");

    return 0;
}