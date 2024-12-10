#include <stdio.h>
#include <thread.h>
#include <time.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,  // tm_year is years since 1900
        .tm_mon = 5,             // tm_mon is month since January (0-11), so 5 is June
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Error setting time\n");
        return 1;
    }

    // Set alarm 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time struct

    if (rtc_set_alarm(&alarm_time, alarm_callback) != 0) {
        printf("Error setting alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Put the main thread to sleep to wait for the alarm
    // The thread will wake up when the alarm is triggered
    while(1) {
        thread_sleep();
    }

    return 0;
}