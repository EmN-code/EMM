# This code initializes RTC with a set time and sets an alarm to trigger after 5 seconds.

#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mutex.h"
#include "periph/gpio.h"
#include "kernel_defines.h"
#include "periph/i2c.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"

static void rtc_alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the RTC time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set the RTC alarm to trigger after 5 seconds
    time.tm_sec += 5;

    while (rtc_set_alarm(&time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm, retrying...\n");
        // Retry setting the alarm after 1 second in case of failure
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Main loop
    while (1) {
        // Do other stuff
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
