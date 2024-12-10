# We're implementing a solution that sets a date and an alarm on an ESP32 using RIOT's RTC APIs.
# The task is to set the current date to June 1, 2024, and create an alarm that triggers a callback function after 5 seconds.
# Once the alarm triggers, it will print "Alarm triggered!".

#include <timex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;   // The callback may pass an argument, mark as unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return -1;
    }

    // Set the RTC time to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900, // tm_year is the number of years since 1900
        .tm_mon  = 6 - 1,       // tm_mon is zero-based
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set an alarm for 5 seconds from now
    struct tm alarm_time;
    if (rtc_get_time(&alarm_time) != 0) {
        printf("Failed to get RTC time\n");
        return -1;
    }

    // Calculate the alarm time
    time_t current_time = mktime(&alarm_time);
    current_time += 5;  // Add 5 seconds
    struct tm *new_alarm_time = gmtime(&current_time);

    if (rtc_set_alarm(new_alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Optional loop to keep the program running
    while (true) {
        xtimer_sleep(1);
    }

    return 0;
}