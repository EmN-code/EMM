#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include "xtimer.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, where 0 = January
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;         // Is DST on? 1 = yes, 0 = no, -1 = unknown

    time_t start_time = mktime(&timeinfo);
    if (start_time == -1) {
        printf("Failed to create the time structure\n");
        return -1;
    }
    printf("Current date is June 1, 2024\n");

    // Set an alarm 5 seconds after the start date
    time_t alarm_time = start_time + 5;

    while (time(NULL) < alarm_time) {
        // Busy-waiting if running without deeper hardware RTC capabilities
        // In a real-world example, this should be handled by sleep functions
    }

    // Trigger the alarm callback function
    alarm_callback(NULL);
    return 0;
}