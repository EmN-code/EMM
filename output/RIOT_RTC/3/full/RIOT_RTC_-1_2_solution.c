#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"

void rtc_callback(void *arg) {
    (void)arg;  // silences the unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Declare and set time to June 1, 2024, 00:00:00
    struct tm time; // Ensure to use struct tm
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;      // 0-indexed, so 5 means June
    time.tm_mday = 1;     // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&(time_t){mktime(&time)});

    // Set the alarm for 5 seconds from the current RTC time
    struct tm alarm_time;
    time_t current_time = rtc_get_time();
    gmtime_r(&current_time, &alarm_time);
    alarm_time.tm_sec += 5; // Add 5 seconds to current time

    rtc_set_alarm(&(time_t){mktime(&alarm_time)}, rtc_callback, NULL);

    printf("Alarm set for 5 seconds from now\n");

    // Keep the program running to catch the alarm
    // Sleep in a low-power mode until an event occurs
    while (1) {
        xtimer_usleep(1000000);  // sleep for 1 second
    }

    return 0;
}