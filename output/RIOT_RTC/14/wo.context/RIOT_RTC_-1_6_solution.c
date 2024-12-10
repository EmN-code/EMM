#include <time.h>
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        puts("RTC initialization failed");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5;            // tm_mon months since January [0-11]
    time.tm_mday = 1;           // day of the month [1-31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    if (rtc_set_time(&time) != 0) {
        puts("Failed to set time");
        return 1;
    }

    // Set an alarm 5 seconds from now
    xtimer_sleep(5);
    rtc_set_alarm(NULL, alarm_callback, NULL);

    // Let the alarm system work
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}