#include <stdio.h>
#include <time.h>     // For time-related functions
#include <unistd.h>   // For the sleep function
#include <periph_conf.h>

void rtc_alarm_callback(void);

void rtc_alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the initial date and time to June 1, 2024, 00:00:00
    struct tm date_time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,            // Months since January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    time_t initial_time = mktime(&date_time);
    rtc_set_time(initial_time);

    // Set an alarm for 5 seconds from the current time
    time_t alarm_time = initial_time + 5;
    rtc_set_alarm(alarm_time, rtc_alarm_callback);

    // Loop indefinitely
    while (1) {
        sleep(1);  // Sleep for one second
    }

    return 0;
}