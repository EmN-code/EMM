// Description of the solution:
// The code initializes a real-time clock (RTC) on an ESP32 using RIOT OS. It sets the RTC date and time to June 1, 2024, 00:00:00, and sets up an alarm to trigger 5 seconds later. When the alarm triggers, it prints "Alarm triggered!".

#include <stdio.h>
#include <periph/rtc.h>
#include <xtimer.h>

// Define callback function for RTC alarm
void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the date to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // year since 1900
    time.tm_mon = 6 - 1;        // months since January
    time.tm_mday = 1;           // day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = 0;          // daylight saving time

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm for 5 seconds later
    xtimer_sleep(5); // Sleep for 5 seconds before setting the alarm

    // Get current time and add 5 seconds for the alarm
    rtc_get_time(&time);
    time.tm_sec += 5;
    mktime(&time);  // Normalize the struct tm

    if (rtc_set_alarm(&time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    while (1) {
        // Add loop to keep main alive
    }

    return 0;
}