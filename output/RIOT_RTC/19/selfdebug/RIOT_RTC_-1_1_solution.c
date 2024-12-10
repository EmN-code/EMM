#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC and set the date
    struct tm date;
    date.tm_sec = 0;
    date.tm_min = 0;
    date.tm_hour = 0;
    date.tm_mday = 1;
    date.tm_mon = 5;  // June (months since January)
    date.tm_year = 2024 - 1900;  // year since 1900
    date.tm_isdst = -1;  // auto-detect Daylight Saving Time

    time_t t = mktime(&date);
    if (t == -1) {
        perror("Failed to set date");
        return EXIT_FAILURE;
    }

    printf("Date set to: %s", ctime(&t));

    // Set RTC alarm to go off after 5 seconds
    struct tm alarm_time = date;
    alarm_time.tm_sec += 5;

    // Normally you would use rtc_set_alarm here
    // For simulation purposes, we instead will use a busy-wait loop
    time_t alarm_t = mktime(&alarm_time);
    if (alarm_t == -1) {
        perror("Failed to set alarm");
        return EXIT_FAILURE;
    }

    printf("Waiting for alarm to trigger...\n");
    while (difftime(alarm_t, time(NULL)) > 0) {
        // Busy wait until alarm time
    }
    rtc_alarm_callback(NULL);

    return EXIT_SUCCESS;
}