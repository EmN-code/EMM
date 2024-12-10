#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"

// Initialize the RTC time and set an alarm 5 seconds later
int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month 0 - 11
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set the alarm time for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    // Set the RTC alarm
    if (rtc_set_alarm(&alarm_time, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    // Alarm callback function
    void alarm_callback(void *arg) {
        (void)arg;
        printf("Alarm triggered!\n");
    }

    // Set the callback function
    rtc_set_alarm_callback(alarm_callback, NULL);

    // Keep running to wait for the alarm
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}