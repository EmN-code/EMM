#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

// Callback function when the alarm is triggered
void rtc_alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;

    // Set date to June 1, 2024 00:00:00
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Month, where 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return -1;
    }

    // Set the alarm to trigger 5 seconds after setting the RTC
    if (rtc_alarm(time(NULL) + 5, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm.\n");
        return -1;
    }

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}