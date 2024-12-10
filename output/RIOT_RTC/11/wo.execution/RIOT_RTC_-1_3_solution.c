#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <periph/i2c.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <periph/gpio.h>
#include <periph/rtc.h>

#define ALARM_IN_SECONDS 5

void alarm_callback(void *arg) {
    (void)arg; // unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 5; // Months since January [0-11]
    timeinfo.tm_mday = 1; // Day of the month [1-31]
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_time_set(&timeinfo);

    // Set an alarm to fire in ALARM_IN_SECONDS
    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + ALARM_IN_SECONDS; // Alarm after 5 seconds
    alarm.callback = alarm_callback;
    alarm.arg = NULL;
    rtc_alarm_set(&alarm);

    // Continue running other process here...
    while (1) {
        // Sleep or perform other tasks
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}