#include <stdio.h>
#include <time.h>
#include <periph_conf.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg;   // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 5,             // Month [0-11] (June)
        .tm_mday = 1,            // Day of the month [1-31]
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&time);

    // Set the alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Infinite loop to keep the program running
    while (1) {
        // Keep the program running and allowing interrupts to process
    }

    return 0;
}