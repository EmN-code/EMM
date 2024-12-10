#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#include <stdio.h>
#include <time.h>

// Callback function for RTC alarm
void alarm_triggered(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Define the time for setting the RTC
    time.tm_year = 2024 - 1900; // tm_year is the year since 1900
    time.tm_mon = 5;            // tm_mon is zero-based, so 5 represents June
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    rtc_init();
    rtc_set_time(&time);

    // Wait for 5 seconds before setting the alarm
    xtimer_sleep(5);

    // Set the alarm callback to trigger after 5 seconds
    rtc_set_alarm(&time, alarm_triggered, NULL);

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}