#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "unistd.h"
#include "netinet/in.h"
#include "arpa/inet.h"

// RTC alarm callback function
void alarm_trigger(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC
    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1;        // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(mktime(&time));

    // Set an alarm 5 seconds from now
    xtimer_sleep(5);

    rtc_set_alarm_relative(5, alarm_trigger, NULL);

    while (1) {
        // Main loop can handle other tasks
        xtimer_sleep(1);
    }

    return 0;
}