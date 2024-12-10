#include <xtimer.h>
#include <stdio.h>
#include <rtc.h>

void alarm_callback(void *arg)
{
    (void)arg; // Unused parameter
    puts("Alarm triggered!");
}

int main(void)
{
    rtc_t rtc;
    rtc_init(&rtc);

    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 6 - 1;         // Month, 0 = January
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set the current date/time
    if (rtc_set_time(&rtc, &timeinfo) != 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Current time after setting
    printf("RTC set to: %s", asctime(&timeinfo));

    // Configure alarm time: 5 seconds after current time
    xtimer_usleep(5 * US_PER_SEC);
    puts("Alarm will trigger in 5 seconds...");

    // Set the alarm and register the callback
    rtc_set_alarm(&rtc, &timeinfo, alarm_callback, NULL);

    // Running loop to keep the main function active
    while (1) {
        xtimer_sleep(10); // Sleep to keep the process active
    }

    return 0;
}