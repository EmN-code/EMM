#include <stdio.h>
#include <timex.h>
#include "periph/rtc.h"

int main(void) {
    struct tm time;
    rtc_t dev = RTC_DEV(0);

    // Initialize RTC
    if (rtc_init(dev) != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set to June 1, 2024
    time.tm_year = 2024-1900; // tm_year is year since 1900
    time.tm_mon = 5;          // tm_mon is month from 0 (Jan) to 11 (Dec)
    time.tm_mday = 1;         // tm_mday is day of the month from 1 to 31
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(dev, &time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Wait 5 seconds
    xtimer_sleep(5);

    // Alarm action
    printf("Alarm triggered!\n");

    return 0;
}