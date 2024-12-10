#include <xtimer.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "periph/rtc.h"

// Alarm callback function
static void alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    memset(&time, 0, sizeof(struct tm));
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month (0-11)
    time.tm_mday = 1;            // Day of the month (1-31)

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set alarm 5 seconds after the set time
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Current time set and alarm scheduled\n");

    // Enter an infinite loop to keep the main function running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}