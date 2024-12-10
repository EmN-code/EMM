#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

void rtc_callback(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    rtc_t dev = RTC_DEV(0);

    // Initialize the RTC device
    if (rtc_init(dev) != 0) {
        printf("Error: unable to initialize RTC\n");
        return 1;
    }

    // Set the date to June 1, 2024, 00:00:00
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, 0-11
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set time using timeinfo
    if (rtc_set_time(dev, &timeinfo) != 0) {
        printf("Error: unable to set time\n");
        return 1;
    }

    // Set the alarm to trigger after 5 seconds
    rtc_t alarm_time;
    alarm_time.tm_sec = 5;  // 5 seconds from now
    alarm_time.tm_min = 0;
    alarm_time.tm_hour = 0;
    alarm_time.tm_mday = 0;
    alarm_time.tm_mon = 0;
    alarm_time.tm_year = 0;

    if (rtc_set_alarm(dev, &alarm_time, rtc_callback) != 0) {
        printf("Error: unable to set alarm\n");
        return 1;
    }

    printf("RTC is set to June 1, 2024, an alarm will trigger in 5 seconds\n");

    // Main loop
    while (1) {
        if (rtc_alarm_pending(dev)) {
            rtc_clear_alarm(dev);
            break;
        }
    }

    return 0;
}