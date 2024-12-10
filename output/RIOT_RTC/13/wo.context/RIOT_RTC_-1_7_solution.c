#include <xtimer.h>  // Required for time operations and setting timers
#include <periph/rtc.h>  // Required for RTC (Real Time Clock) operations
#include <stdio.h>  // Required for standard input and output operations

// Define the time structure for the date June 1, 2024
static struct tm target_time = {
    .tm_year = 2024 - 1900, // Year since 1900
    .tm_mon = 6 - 1, // Months since January (0-11)
    .tm_mday = 1, // Day of the month (1-31)
    .tm_hour = 0,
    .tm_min = 0,
    .tm_sec = 0
};

// Function to be called when the alarm is triggered
void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm current_time;
    time_t target_epoch;

    // Initialize RTC with the target date
    rtc_set_time(&target_time);

    // Get the current time from RTC to validate setting
    rtc_get_time(&current_time);

    // Calculate the future epoch time for the alarm (5 seconds later)
    target_epoch = mktime(&current_time) + 5;

    // Set an RTC alarm for 5 seconds from now
    rtc_set_alarm(target_epoch, alarm_callback, NULL);

    // Keep the main function running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}