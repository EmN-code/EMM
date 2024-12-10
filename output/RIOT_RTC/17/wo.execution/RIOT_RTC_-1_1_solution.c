#include <stdio.h>
#include "rtc.h"
#include "thread.h"

int main(void) {
    printf("Setting RTC to June 1, 2024, 00:00:00\n");

    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 5;  // June
    timeinfo.tm_mday = 1; 
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);

    printf("Setting alarm for 5 seconds later\n");

    // Prepare the alarm time which is 5 seconds later than the current time
    timeinfo.tm_sec += 5;

    // Set the RTC alarm
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    // Sleep in main function to ensure the program keeps running
    while (1) {
        thread_yield();
    }

    return 0;
}

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}