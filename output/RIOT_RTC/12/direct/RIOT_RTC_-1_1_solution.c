#include <xtimer.h> // For timer functions
#include <stdio.h>   // For standard input and output
#include <string.h>  // For string operations
#include <time.h>    // For handling time and date

// Main function
int main(void) {
    // Initialize and configure the RTC
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Set year (years since 1900)
    timeinfo.tm_mon = 6 - 1;         // Set month (0-11, where 0 is January)
    timeinfo.tm_mday = 1;            // Set day of the month (1-31)
    timeinfo.tm_hour = 0;            // No specific hour set
    timeinfo.tm_min = 0;             // No specific minute set
    timeinfo.tm_sec = 0;             // No specific second set

    // Convert struct tm to time_t
    time_t current_time = mktime(&timeinfo);

    // Display the current set date/time
    printf("Current date/time set: %s", asctime(&timeinfo));

    // Set an alarm for 5 seconds later
    time_t alarm_time = current_time + 5; // Set alarm for 5 seconds after the current time

    // Infinite loop to wait for the alarm
    while (1) {
        time_t now = time(NULL);  // Get current time
        if (now >= alarm_time) {  // Check if alarm time has been reached
            printf("Alarm triggered!\n");  // Print alarm message
            break;  // Exit the loop after triggering alarm
        }
        xtimer_sleep(1);  // Delay for 1 second
    }

    return 0;  // Program completed
}