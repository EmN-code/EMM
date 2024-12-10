#include <time.h>
#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void alarm_handler(void *arg)
{
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm timeinfo;
    time_t epoch_time;

    // Set the time to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 6 - 1; // Months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Convert tm structure to epoch time
    epoch_time = mktime(&timeinfo);
    if (epoch_time == -1) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time((struct rtc_time*)&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm 5 seconds from "now"
    epoch_time += 5;
    gmtime_r(&epoch_time, &timeinfo);

    // Set RTC alarm
    rtc_set_alarm((struct rtc_time*)&timeinfo, alarm_handler, NULL);

    printf("Alarm set for 5 seconds from set time!\n");

    // Wait until alarm is triggered
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}