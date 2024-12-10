#include <xtimer.h>
#include <stdio.h>
#include "periph/rtc.h"

void set_alarm(rtc_time_t *time) {
    // Adjust time by adding 5 seconds for the alarm
    time->tm_sec += 5;
    if (time->tm_sec >= 60) {
        time->tm_sec -= 60;
        time->tm_min += 1;
    }
    if (time->tm_min >= 60) {
        time->tm_min -= 60;
        time->tm_hour += 1;
    }
    if (time->tm_hour >= 24) {
        time->tm_hour -= 24;
        time->tm_mday += 1;
    }
    // Adjust as necessary for month/year changes

    // Set the RTC alarm
    rtc_set_alarm(time);
}

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC and set time to June 1, 2024, 00:00:00
    rtc_time_t time = {
        .tm_year = 2024,
        .tm_mon = 6,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };

    // Set the time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set the alarm
    set_alarm(&time);

    // Register the alarm callback
    rtc_cb_t cb = { .cb = alarm_callback, .arg = NULL };
    rtc_set_alarm_cb(&cb);

    // Loop forever to keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}