#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "rtc.h"
#include "kernel.h"

static void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,   // Month is 0-11
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_init();

    // Set the current date and time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    struct tm alarm_time;
    rtc_get_time(&alarm_time);

    // Set an alarm for 5 seconds from the current time
    alarm_time.tm_sec += 5;
    
    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    while (1) {
        // Keep running other tasks if needed
    }
    return 0;
}