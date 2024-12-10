#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include <stdio.h>

#define ALARM_OFFSET_SEC 5  // Offset for alarm in seconds

// RTC alarm callback function
void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_t dev = RTC_DEV(0);

    // Set the RTC time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;             // Month 0-11 (0 = January)
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init(dev) != 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    if (rtc_set_time(dev, &time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set RTC alarm 5 seconds from now
    xtimer_sleep(ALARM_OFFSET_SEC);

    // Register the callback for the alarm
    if (rtc_set_alarm(dev, ALARM_OFFSET_SEC, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("RTC alarm set!\n");

    // Main loop
    while (1) {
        // Normally you'd have more application logic here
    }

    return 0;
}