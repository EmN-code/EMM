#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define RTC_INTERVAL (5U)  // Define the interval in seconds for the RTC alarm

static void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Variables for RTC
    struct tm time;

    // Set RTC date and time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900;
    time.tm_mon  = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Setting RTC time failed\n");
        return -1;
    }

    // Prepare and set RTC alarm
    ztimer_sleep(ZTIMER_MSEC, RTC_INTERVAL * 1000U);
    rtc_alarm_callback(NULL);

    return 0;
}
