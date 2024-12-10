#include <stdio.h>
#include <time.h>
#include <periph/rtc.h>
#include <xtimer.h>

// Define the callback function for the alarm
void alarm_callback(void *arg) {
    (void) arg; // unused
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5; // 0-based, so 5 is June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set the current time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time!\n");
        return 1;
    }

    // Set an alarm for 5 seconds later
    xtimer_sleep(5);

    // Get current time and adjust for alarm
    rtc_get_time(&time);
    time.tm_sec += 5;  // Trigger in 5 seconds
    mktime(&time);     // Normalize the time structure

    // Set the RTC alarm
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm!\n");
        return 1;
    }

    printf("Alarm set for 5 seconds later. Waiting...\n");

    // Wait indefinitely to allow alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}