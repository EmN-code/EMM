#include <stdio.h>
#include <time.h>
#include "rtc.h"
#include "xtimer.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;  // suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_alarm_cb_t callback = rtc_alarm_callback;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // 0-based month
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize the RTC
    rtc_init();

    // Set the RTC to the specified date
    rtc_set_time(&timeinfo);

    // Set the alarm to trigger 5 seconds after the current RTC time
    rtc_set_alarm(&timeinfo, callback, NULL);

    // Wait indefinitely, keeping the main thread active
    while (1) {
        xtimer_sleep(10); // Sleep for 10 seconds in each loop iteration to reduce CPU usage
    }

    return 0;
}