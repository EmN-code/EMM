#include <stdio.h>
#include <time.h>

// RTC and alarm configuration on RIOT for ESP32
void alarm_triggered(void)
{
    puts("Alarm triggered!");
}

int main(void)
{
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, where 0 = January
    timeinfo.tm_mday = 1;           // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC here (pseudo-code, replace with actual initialization)
    // rtc_init();
    // rtc_set_time(&timeinfo);

    // Set an alarm for 5 seconds later
    struct timespec alarm_time = {.tv_sec = 5, .tv_nsec = 0};
    // rtc_set_alarm(&alarm_time, alarm_triggered);  // Again, pseudo-code

    // Simulating infinite loop to keep program running and listening for alarms
    while (1) {}

    return 0;
}