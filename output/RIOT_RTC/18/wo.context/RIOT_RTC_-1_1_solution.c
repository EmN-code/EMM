#include <stdio.h>
#include <rtc.h> // ensure the RTC library is included
#include <time.h>

void alarm_cb(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 5; // Months are 0-11
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC
    rtc_init();

    // Set the current time
    rtc_set_time(&timeinfo);

    // Set an alarm to trigger 5 seconds later
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_cb, NULL);

    // Infinite loop to keep the main program running
    while (1) {
        // In a real scenario, we could put the system to sleep to save power
        // and wake up when the alarm triggers.
    }
    return 0;
}
