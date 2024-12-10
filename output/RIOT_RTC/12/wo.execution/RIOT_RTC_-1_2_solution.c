#include <time_units.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1; // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time.\n");
        return 1;
    }

    // Set alarm to 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm.\n");
        return 1;
    }

    // Sleep for a minute to allow the alarm to trigger
    usleep(60 * US_PER_SEC);

    return 0;
}