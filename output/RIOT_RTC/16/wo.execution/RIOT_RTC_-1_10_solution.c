#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the current time to June 1, 2024
    struct tm current_time;
    current_time.tm_year = 2024 - 1900;  // Year since 1900
    current_time.tm_mon = 6 - 1;         // Month (0-11)
    current_time.tm_mday = 1;            // Day of the month
    current_time.tm_hour = 0;
    current_time.tm_min = 0;
    current_time.tm_sec = 0;
    time_t t = mktime(&current_time);
    struct timeval now = { .tv_sec = t };

    // Set RTC time
    settimeofday(&now, NULL);

    // Set an alarm 5 seconds from "now"
    xtimer_sleep(5);

    // Call the alarm callback
    alarm_callback(NULL);

    return 0;
}