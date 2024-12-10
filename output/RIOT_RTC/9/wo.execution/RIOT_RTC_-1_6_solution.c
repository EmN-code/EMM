#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "msg.h"
#include "unistd.h"

void rtc_alarm_triggered(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize the RTC
    printf("Initializing RTC...\n");
    rtc_init();

    // Set the RTC time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Month, 0 = January
    time.tm_mday = 1;           // Day of month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Use custom RTC set time logic
    if (rtc_set_time(&time) < 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set an alarm for 5 seconds from the current RTC time
    if (rtc_set_alarm_relative(5, rtc_alarm_triggered, NULL) < 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Sleep to keep the process alive
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}