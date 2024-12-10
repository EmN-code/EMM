#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

// Callback function triggered by the RTC alarm
void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set initial date and time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon  = 6 - 1,        // Month, 0-based
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }
    rtc_set_time(&time);
    printf("RTC set to: %s\n", asctime(&time));

    // Set an alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }
    printf("Alarm set for 5 seconds later\n");

    // Main loop
    while (1) {
        xtimer_sleep(10);  // Sleep or perform other tasks
    }

    return 0;
}