#include <time.h>
#include <stdio.h>

void alarm_callback(void *arg)
{
    (void)arg; // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) 
{
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // tm_year is the number of years since 1900
        .tm_mon  = 5,           // tm_mon is the month since January, 0-11
        .tm_mday = 1,           // Day of the month, 1-31
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    // Set the RTC time to June 1, 2024
    rtc_set_time(&timeinfo); // assuming rtc_set_time takes a tm structure pointer

    // Set an alarm for 5 seconds later
    timeinfo.tm_sec += 5;

    // Normalize the time structure
    mktime(&timeinfo);

    // Start RTC alarm with a callback
    rtc_set_alarm(&timeinfo, alarm_callback, NULL); // assuming rtc_set_alarm function signature

    while(1) {
        // Main loop doing other tasks
    }

    return 0;
}
