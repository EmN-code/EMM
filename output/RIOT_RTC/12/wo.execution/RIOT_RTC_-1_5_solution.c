#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "unistd.h" 
#include "stdatomic.h"

static void alarm_callback(void *arg)
{
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Error: RTC initialization failed!\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error: failed to set RTC time!\n");
        return 1;
    }

    // Set alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep to simulate delay for testing purpose
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL, 0) != 0) {
        printf("Error: failed to set RTC alarm!\n");
        return 1;
    }

    // Fake delay to let the alarm trigger
    ztimer_sleep(ZTIMER_SEC, 10);

    return 0;
}