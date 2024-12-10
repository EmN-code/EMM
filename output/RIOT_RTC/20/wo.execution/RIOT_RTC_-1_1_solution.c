#include <stdio.h>
#include "rtc.h"
#include "timex.h"
#include "ztimer.h"

// RTC alarm callback function
void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set RTC time to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&time);

    // Set RTC alarm for 5 seconds later
    rtc_alarm_cb_t callback = rtc_alarm_callback;
    rtc_set_alarm(NULL, callback, NULL);
    ztimer_sleep(ZTIMER_SEC, 5);

    while (1) {
        // Main loop can be used for other tasks
    }

    return 0;
}