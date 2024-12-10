// This example demonstrates how to use the RTC alarm function in RIOT on an ESP32 platform.
// The RTC is initialized, the current date is set to June 1, 2024, and an alarm is set to trigger after 5 seconds.
// Upon triggering, the alarm prints "Alarm triggered!".

#include "thread.h"
#include "periph/rtc.h"
#include "event/thread.h"

void alarm_callback(void *arg)
{
    (void)arg;  // Suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC
    rtc_init();

    // Set the current date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month, 0 = January, so 5 = June
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;  // Add 5 seconds to the current time
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Go to sleep while waiting for the alarm
    thread_sleep();

    return 0;
}