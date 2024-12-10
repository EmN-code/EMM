#include <time.h>
#include "xtimer.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 5;  // Months are 0-11, so 5 = June
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1; // No daylight saving time

    // Assume rtc_init() to initialize RTC, if not already done
    // rtc_set_time(&timeinfo);  // Set the RTC's time

    // Set an alarm to trigger after 5 seconds
    timeinfo.tm_sec += 5;
    if (rtc_set_alarm(&timeinfo, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
    }

    // Enter a low-power loop, or continue with other operations
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}