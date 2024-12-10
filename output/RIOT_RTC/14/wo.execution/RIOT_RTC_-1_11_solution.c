#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <msg.h>
#include <stdlib.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_init();

    // Set the current time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // Month 0-11, so 5 for June
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Setup alarm time to 5 seconds later
    time.tm_sec += 5;
    mktime(&time); // Normalize the time

    rtc_set_alarm(&time, alarm_callback, NULL);

    while (1) {
        // Keep the main thread running
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}