#include <stdio.h>
#include <time.h> // Correct header for standard time functions
#include <xtimer.h> // For delay functions
#include <periph/rtc.h> // Include the correct RTC peripheral header

#define ALARM_INTERVAL_SEC 5 // Alarm interval in seconds

void alarm_callback(void *arg) {
    (void)arg; // Suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC device
    int res = rtc_init();
    if (res != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Define a tm structure for the desired date
    struct tm time;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_mday = 1; // Day of the month (1-31)
    time.tm_mon = 5; // Months since January (0-11)
    time.tm_year = 124; // Years since 1900

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Calculate alarm time
    struct tm alarm_time = time;
    alarm_time.tm_sec += ALARM_INTERVAL_SEC;
    mktime(&alarm_time);  // Normalize the time structure

    // Set the RTC alarm
    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for %d seconds from now\n", ALARM_INTERVAL_SEC);

    // Main loop
    while (1) {
        // We can add logic that depends on rtc_alarm_pending() if needed
        xtimer_sleep(1);
    }

    return 0;
}