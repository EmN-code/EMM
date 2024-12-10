#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "ztimer.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Define time structure and set the date to June 1, 2024
    struct tm set_time;
    set_time.tm_year = 2024 - 1900; // Year since 1900
    set_time.tm_mon = 6 - 1;        // Month, 0-11
    set_time.tm_mday = 1;           // Day of the month
    set_time.tm_hour = 0;           // Hour since midnight
    set_time.tm_min = 0;            // Minutes after the hour
    set_time.tm_sec = 0;            // Seconds after the minute
    set_time.tm_isdst = -1;         // No daylight saving time

    // Convert struct tm to time_t
    time_t future_time = mktime(&set_time);

    // Calculate current time
    time_t current_time;
    time(&current_time);

    // Calculate difference in seconds from current time
    double seconds_to_wait = difftime(future_time, current_time);

    // If the set time is in the past, adjust it for the next cycle
    if (seconds_to_wait < 0) {
        printf("Specified time is in the past. Adjusting for the next cycle.\n");
        future_time = current_time + 5; // Set alarm 5 seconds from now
    }

    // Trigger alarm 5 seconds after the specified time
    ztimer_sleep(ZTIMER_USEC, (seconds_to_wait + 5) * 1000000U);
    alarm_callback();

    return 0;
}