/*
This code sets up the RTC on an ESP32 using the RIOT operating system. It initializes the RTC with a specific date (June 1, 2024) and sets an alarm to trigger after 5 seconds. When the alarm is triggered, it prints "Alarm triggered!".
*/

#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

// Structure to hold date information
rtc_time_t time_now = {
    .tm_year = 2024,   // Year
    .tm_mon  = 5,      // Month (0 = January, thus 5 = June)
    .tm_mday = 1,      // Day of the month
    .tm_hour = 0,      // Hour
    .tm_min  = 0,      // Minute
    .tm_sec  = 0       // Second
};

static void rtc_alarm_callback(void *arg)
{
    (void)arg;   // Unused
    puts("Alarm triggered!");
}

int main(void)
{
    // Initialize the RTC
    rtc_init();

    // Set the current time
    if (rtc_set_time(&time_now) != 0) {
        puts("Error: unable to set RTC time");
        return 1;
    }

    // Get the current time to set the alarm
    rtc_time_t alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;  // Set the alarm to 5 seconds from "now"

    // Adjust the time and carry over if needed
    if (alarm_time.tm_sec >= 60) {
        alarm_time.tm_sec -= 60;
        alarm_time.tm_min += 1;
    }

    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        puts("Error: unable to set RTC alarm");
        return 1;
    }

    // Sleep indefinitely, waiting for the alarm
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}