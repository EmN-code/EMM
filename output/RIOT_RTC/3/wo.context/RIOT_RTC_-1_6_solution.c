#include <stdio.h>
#include <xtimer.h>
#include <time.h>
#include <periph/rtc.h>

void alarm_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5;            // June (months since January 0-11)
    time.tm_mday = 1;           // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }
    else {
        printf("Time set successfully\n");
    }

    struct tm alarm_time;
    xtimer_sleep(5);  // Delay to set alarm
    rtc_get_time(&alarm_time);  // Get current time
    alarm_time.tm_sec += 5;  // Set alarm 5 seconds later

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }
    else {
        printf("Alarm set successfully\n");
    }

    while (1) {
        xtimer_sleep(1);  // To keep main running
    }
    return 0;
}