#include <xtimer.h>
#include <msg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <periph/rtc.h>
#include <stdio.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

void rtc_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return 1;
    }
    
    // Set the date: June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;  // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1; // Not considering daylight saving time

    rtc_set_time(&time);
    
    // Set an alarm for 5 seconds later
    rtc_set_alarm_relative(5, rtc_callback, NULL);
    
    // Main loop
    while (1) {
        xtimer_sleep(1);
        // Other tasks can be processed here
    }
    return 0;
}