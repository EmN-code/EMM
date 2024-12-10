#include <stdio.h> // For printf function
#include <time.h>  // For handling time functions
#include "periph/rtc.h" // Correct header file for RTC

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));
    
    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 5; // Month, where 0 = jan
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the current time
    rtc_set_time(&timeinfo);
  
    // Set alarm to trigger after 5 seconds
    rtc_set_alarm(0, rtc_get_time() + 5, rtc_alarm_cb, NULL);

    // Simulate waiting for the alarm
    while (1) {
        // Infinite loop to keep the program running
    }

    return 0;
}
