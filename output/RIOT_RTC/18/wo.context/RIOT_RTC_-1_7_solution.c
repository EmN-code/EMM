#include <stdio.h>  // Standard I/O for printing messages
#include <rtc.h>    // RTC peripheral (hypothetical, make sure it's correct for your use)
#include <xtimer.h> // Timer functions to set the delay for the alarm

// Callback function for the RTC alarm
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC and set the time
    struct tm date;
    date.tm_year = 2024 - 1900; // Year since 1900
    date.tm_mon = 6 - 1;        // Month, 0 - January
    date.tm_mday = 1;           // Day of the month
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    
    // Set the RTC time
    rtc_set_time(&date);

    // Set an alarm to trigger after 5 seconds
    xtimer_sleep(5); // Wait for 5 seconds
    rtc_set_alarm(0, alarm_callback, NULL);

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1); // Sleep to prevent busy waiting
    }

    return 0;
}