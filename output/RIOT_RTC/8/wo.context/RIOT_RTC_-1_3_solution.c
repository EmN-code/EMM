#include <xtimer.h>  // Include RIOT's xtimer library for timer functions
#include <time.h>    // Include standard time library for struct tm and time functions
#include <stdio.h>   // For standard input and output functions
#include <rtc.h>     // Include RTC functionalities from RIOT

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");  // Print message when alarm is triggered
}

int main(void) {
    struct tm time;  // Time structure to hold date and time
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month, where January is 0
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;            // Hour since midnight
    time.tm_min = 0;             // Minutes past the hour
    time.tm_sec = 0;             // Seconds past the minute
    
    // Set the current time in the RTC
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set an alarm for 5 seconds from now
    xtimer_sleep(5);  // Wait for 5 seconds before setting the alarm

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
