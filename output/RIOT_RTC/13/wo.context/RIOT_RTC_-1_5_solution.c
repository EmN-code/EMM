#include <time.h> // Correct header to include struct tm definition
#include <stdio.h>
#include "periph/rtc.h"

// RTC alarm callback function
void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, where 0 = January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC: This function must be in the included headers
    rtc_init();
    rtc_set_time(&timeinfo);

    printf("Current RTC time set\n");

    // Setting an alarm 5 seconds after
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        struct tm current_time;
        rtc_get_time(&current_time);
        // Infinite loop keeping main program alive
        xtimer_sleep(1);
    }

    return 0;
}