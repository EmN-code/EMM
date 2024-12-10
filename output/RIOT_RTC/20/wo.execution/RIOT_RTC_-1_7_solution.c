#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

#define ALARM_SECONDS_OFFSET 5

void alarm_triggered(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_init();
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where 0 = jan
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };
    rtc_set_time(&time);
    // Here, we creatively handle the alarm based on the limitations
    // Direct usage of rtc_set_alarm_relative is replaced by manual time checking

    struct tm current_time;
    while (1) {
        rtc_get_time(&current_time);
        if ((current_time.tm_sec == ALARM_SECONDS_OFFSET) && (current_time.tm_min == 0) && (current_time.tm_hour == 0) && (current_time.tm_mday == 1) && (current_time.tm_mon == 6 - 1) && (current_time.tm_year == 2024 - 1900)) {
            alarm_triggered();
            break; // Exit loop after triggering
        }
        xtimer_sleep(1);
    }

    return 0;
}