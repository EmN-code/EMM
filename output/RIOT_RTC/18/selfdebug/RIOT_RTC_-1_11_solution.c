#include <time.h>
#include <stdio.h>
#include <xtimer.h>
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm Example\n");
    
    // Initialize RTC
    rtc_init();

    // Set date to June 1, 2024
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 5;  // June (months since January 0-11)
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time(&timeinfo);

    // Get current RTC time
    rtc_get_time(&timeinfo);
    printf("Current time is: %s", asctime(&timeinfo));

    // Set an alarm for 5 seconds after the current time
    timeinfo.tm_sec += 5;
    mktime(&timeinfo); // Normalize the time structure
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    // Wait indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}