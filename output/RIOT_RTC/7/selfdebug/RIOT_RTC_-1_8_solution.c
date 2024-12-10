#include <stdio.h>
#include <time.h>

// Function prototypes
void rtc_set_time(const struct tm *time);
void rtc_set_alarm(struct tm *time, void (*cb)(void *), void *arg);
void alarm_callback(void *arg);

int main() {
    struct tm time = {0};
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // 0-based (0 = January, 5 = June)
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set alarm 5 seconds after the epoch time set above
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);
    
    // Simulate waiting for the alarm to trigger
    while (1) {
        // Main loop does nothing, just wait
    }

    return 0;
}

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

// Mock implementations of rtc functions
void rtc_set_time(const struct tm *time) {
    // This would set the time on the RTC hardware
}

void rtc_set_alarm(struct tm *time, void (*cb)(void *), void *arg) {
    // This would set the alarm on the RTC hardware
    // Simulate callback directly for testing purposes
    cb(arg);
}