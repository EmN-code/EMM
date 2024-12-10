#include <stdio.h>
#include <string.h>
#include <time.h>
#include <xtimer.h>
#include <msg.h>

#define ALARM_OFFSET 5

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 5;            // tm_mon is 0-11, so June is 5
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    
    // Initialize RTC to set the time
    if (!rtc_init()) {
        printf("RTC Initialization failed\n");
        return -1;
    }
    rtc_set_time(&timeinfo);

    // Get the current time to calculate alarm time
    rtc_get_time(&timeinfo);
    timeinfo.tm_sec += ALARM_OFFSET; // Set alarm for 5 seconds later

    // Set the RTC alarm
    rtc_set_alarm(&timeinfo, rtc_alarm_callback, NULL);
    
    while (1) {
        // Main loop does nothing, waits for the alarm interrupt
        xtimer_sleep(1);
    }
    return 0;
}