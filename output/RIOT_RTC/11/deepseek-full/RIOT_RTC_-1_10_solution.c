
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <periph/rtc.h>
#include <xtimer.h>

#define ALARM_DELAY_SECONDS 5

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    rtc_init();

    struct tm time_struct = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 6 - 1,        // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-59)
    };

    rtc_set_time(&time_struct);

    xtimer_t alarm_timer;
    xtimer_set(&alarm_timer, (ALARM_DELAY_SECONDS * US_PER_SEC), alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
