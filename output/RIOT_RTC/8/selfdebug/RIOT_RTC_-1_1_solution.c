#include <stdio.h>
#include <time.h>

void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Set the initial RTC time (June 1, 2024, 00:00:00)
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, since January (0-11)
    time.tm_mday = 1;            // Day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);
    
    // Set the RTC alarm after 5 seconds
    time.tm_sec += 5;
    rtc_set_alarm(&time, rtc_alarm_callback, NULL);

    // Keep the CPU running to ensure the alarm can trigger
    while (1) {
        // Simulate idle or low-power mode
    }

    return 0;
}