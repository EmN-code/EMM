#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

static void alarm_callback(void *arg) {
    (void)arg; // unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Setup time for June 1, 2024, 00:00:00
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // tm_year is the year since 1900
        .tm_mon = 5,            // June (0-based, so 5 is June)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set the current time
    rtc_set_time(&timeinfo);

    // Set the alarm 5 seconds from now
    xtimer_sleep(5);
    rtc_set_alarm(NULL, alarm_callback, NULL);

    while (1) {
        // Enter low-power mode
        xtimer_sleep(1);
    }

    return 0;
}