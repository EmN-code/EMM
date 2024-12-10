
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <stdbool.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define ALARM_DELAY_SECONDS 5

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm time_struct = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 5,            // months since January (0-11)
        .tm_mday = 1,           // day of the month (1-31)
        .tm_hour = 0,           // hours since midnight (0-23)
        .tm_min = 0,            // minutes after the hour (0-59)
        .tm_sec = 0             // seconds after the minute (0-60)
    };
    rtc_set_time(&time_struct);

    // Set an alarm to trigger after 5 seconds
    xtimer_t alarm_timer;
    xtimer_set(&alarm_timer, ALARM_DELAY_SECONDS * US_PER_SEC, alarm_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
