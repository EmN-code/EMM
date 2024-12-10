#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

// Alarm callback function
void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() < 0) {
        printf("Error initializing RTC\n");
        return -1;
    }
    
    // Set the time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 6 - 1;        // tm_mon is months since January (0-11)
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    if (rtc_set_time(&time) < 0) {
        printf("Error setting time\n");
        return -1;
    }

    // Set an alarm 5 seconds later
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, alarm_callback, NULL) < 0) {
        printf("Error setting alarm\n");
        return -1;
    }

    printf("RTC time set to June 1, 2024, and alarm set for 5 seconds later.\n");

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}