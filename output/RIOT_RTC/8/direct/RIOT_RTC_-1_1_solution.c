#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

#define ALARM_OFFSET_SEC 5

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    printf("Setting RTC and scheduling an alarm\n");

    // Define the date to set: June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,            // Months since January [0-11]
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Error: Could not set RTC time\n");
        return 1;
    }

    // Schedule the alarm 5 seconds after the current time
    struct tm alarm_time = time;
    alarm_time.tm_sec += ALARM_OFFSET_SEC;

    // Normalize the time structure in case seconds overflowed
    mktime(&alarm_time);

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Error: Could not set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds later\n");

    // Keep the main thread alive to catch the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}