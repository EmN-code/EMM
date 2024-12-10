#include "sys/xtimer.h"
#include "time.h"
#include "periph/rtc.h"
#include "stdio.h"

void set_rtc_time(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month 0-11
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    /* Convert tm structure to epoch time */
    time_t epoch_time = mktime(&time);

    /* Set the RTC time */
    if (rtc_set_time(epoch_time) != 0) {
        puts("Failed to set RTC time.");
        return;
    }
}

void set_rtc_alarm(void) {
    /* Set alarm to trigger 5 seconds after current time */
    time_t alarm_time = rtc_get_time() + 5;
    if (rtc_set_alarm(alarm_time, alarm_handler, NULL) != 0) {
        puts("Failed to set RTC alarm.");
    }
}

void alarm_handler(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    set_rtc_time();
    set_rtc_alarm();

    /* Main loop */
    while (1) {
        /* Keep the program running */
        xtimer_sleep(1);
    }

    return 0;
}