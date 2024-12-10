#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "unistd.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "net/emcute.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    /* Initialize the RTC */
    rtc_init();

    /* Set the current time to June 1, 2024, 00:00:00 */
    struct tm time_current;
    time_current.tm_year = 2024 - 1900; // Year since 1900
    time_current.tm_mon = 5;  // Month, where 0 = jan
    time_current.tm_mday = 1; // Day of the month
    time_current.tm_hour = 0;
    time_current.tm_min = 0;
    time_current.tm_sec = 0;
    rtc_set_time(&time_current);

    /* Set the alarm to trigger after 5 seconds */
    rtc_set_alarm_rltimer(5, rtc_alarm_cb, NULL);

    /* Busy-wait loop to keep the program running and allow the alarm to trigger */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
