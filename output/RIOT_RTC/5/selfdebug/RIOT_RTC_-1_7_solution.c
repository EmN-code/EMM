#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

// Function to be called when alarm triggers
void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = { 0 };
    rtc_cc_t alarm_time;
    
    // Set the initial time to 2024-06-01 00:00:00
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 5;             // Month, where 0 = January
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set the alarm time (5 seconds after the initial time)
    alarm_time = rtc_get_counter() + 5;

    if (rtc_set_alarm(alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now.\n");
    
    // Enter an idle loop
    while (1) {
        // Here you can put the device into a low power mode
        // to save energy while waiting for the alarm
    }
    
    return 0;
}
