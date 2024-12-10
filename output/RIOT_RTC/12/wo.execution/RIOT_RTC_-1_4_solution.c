#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "stdlib.h"

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    puts("RTC Alarm Example");

    // Set the target time for June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month, 0 - jan
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init() != 0) {
        puts("Failed to initialize RTC");
        return 1;
    }

    timex_t timex_time;
    timex_time.seconds = mktime(&time); // Set RTC to the specified time

    if (rtc_set_time(&timex_time) != 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Set alarm to trigger 5 seconds after the set time
    timex_time.seconds += 5;

    rtc_alarm_t alarm;
    alarm.time = timex_time;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        puts("Failed to set alarm");
        return 1;
    }

    while (1) {
        xtimer_sleep(60); // Sleep to keep the program running
    }

    return 0;
}