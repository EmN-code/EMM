#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "periph/rtc.h"
#include <stdio.h>

#define ALARM_SECONDS (5)

static void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Set the RTC to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900,
        .tm_mon  = 5,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    rtc_set_time(&time);

    // Set an alarm for ALARM_SECONDS seconds in the future
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += ALARM_SECONDS;
    mktime(&alarm_time); // Normalize the time structure

    ztimer_t alarm_timer;
    alarm_timer.callback = alarm_callback;
    alarm_timer.arg = NULL;
    ztimer_set(ZTIMER_SEC, &alarm_timer, ALARM_SECONDS);

    while (1) {
        // Main loop
        xtimer_sleep(1);
    }

    return 0;
}