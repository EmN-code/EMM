#include <time.h>
#include <stdio.h>
#include <xtimer.h>

// Placeholder for RTC library includes (e.g., #include <periph/rtc.h>)

#define ALARM_INTERVAL 5  // Alarm will be set for 5 seconds later

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    uint16_t milliseconds;

    // Set the current date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;  // Months since January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set RTC time
    if (rtc_get_time_ms(&time, &milliseconds) != 0) {
        printf("Failed to get RTC time\n");
        return -1;
    }

    // Calculate alarm time
    alarm_time = time;
    alarm_time.tm_sec += ALARM_INTERVAL;

    // Set RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Keep the program running to catch the alarm
    while (1) {
        xtimer_sleep(10);  // Sleep to prevent busy-waiting
    }

    return 0;
}