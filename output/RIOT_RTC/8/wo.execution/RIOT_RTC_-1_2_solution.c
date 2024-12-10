#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_init();

    // Set current date to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month (0-11)
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm for 5 seconds from now
    rtc_get_time(&time);  // get the current time again to add 5 seconds
    time.tm_sec += 5;

    // Callback for the alarm
    rtc_set_alarm(&time, alarm_callback, NULL);

    while (1) {
        // Main loop can perform other tasks
    }
    return 0;
}