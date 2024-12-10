#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <msg.h>
#include <kernel_defines.h>
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t alarm_cb = alarm_callback;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;  // 0-based months
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an RTC alarm for 5 seconds later
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    ztimer_sleep(ZTIMER_USEC, 5000000); // Simulate delay for the purpose of demonstration
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds later\n");
    return 0;
}