#include <time.h>
#include <stdio.h>
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set the date and time to June 1, 2024, 00:00:00
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, where 0 = January
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time(&timeinfo);

    // Set alarm for 5 seconds later
    timeinfo.tm_sec += 5;
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    // Enter a sleep state or low power mode if possible
    // Your hardware may support different power-saving features
    while (1) {
        // sleep(); or a similar power-saving function
    }

    return 0;
}