#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <periph/rtc.h>

void alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    rtc_get_time(&time);
    printf("Current date and time is: %s", asctime(&time));

    // Set the new date to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 6 - 1; // tm_mon is month since January, 0-11
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting time\n");
        return 1;
    }

    printf("New date and time set to: %s", asctime(&time));

    // Set alarm to 5 seconds from now
    struct tm alarm_time = time;
    rtc_get_time(&alarm_time); // Update alarm_time to the current time

    alarm_time.tm_sec += 5; // Add 5 seconds
    mktime(&alarm_time); // Normalize time structure

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Error setting alarm\n");
        return 1;
    }

    printf("Alarm set for: %s", asctime(&alarm_time));

    // Wait for the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}