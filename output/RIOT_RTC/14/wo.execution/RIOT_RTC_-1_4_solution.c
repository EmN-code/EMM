#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "stdatomic.h"

#define ALARM_DELAY (5U)

static void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time = { .tm_year = 2024 - 1900, // Year since 1900
                       .tm_mon  = 6 - 1,        // Month 0-11
                       .tm_mday = 1,           // Day of the month 1-31
                       .tm_hour = 0,
                       .tm_min  = 0,
                       .tm_sec  = 0 };

    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return -1;
    }

    rtc_set_time(&time);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    rtc_set_alarm_reltimer(ALARM_DELAY * 1000, alarm_callback, NULL);

    while (1) {
        // keep the program running to allow the alarm to trigger
    }

    return 0;
}