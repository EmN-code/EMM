#include <stdio.h>
#include <xtimer.h>
#include "periph/rtc.h"

void rtc_alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_init();

    // Set the date to June 1, 2024, 00:00:00
    time.tm_year = 2024; // Year
    time.tm_mon = 5;    // Month 0-11
    time.tm_mday = 1;   // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm 5 seconds later
    xtimer_sleep(5);

    // Retrieve current time and add 5 seconds
    rtc_get_time(&time);
    time.tm_sec += 5; // Add 5 seconds

    // Set the RTC alarm to trigger after 5 seconds
    rtc_set_alarm(&time, rtc_alarm_handler, NULL);

    while (1) { 
        // Keeping the main function running
        xtimer_sleep(1);
    }
    return 0;
}