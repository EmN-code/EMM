#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"
#include "msg.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mbox.h"
#include "kernel_defines.h"

#define ALARM_OFFSET_SEC   5
#define ALARM_MESSAGE      "Alarm triggered!"

static void alarm_handler(void *arg) {
    (void)arg;
    puts(ALARM_MESSAGE);
}

int main(void) {
    rtc_init();

    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // Month (0-11)
        .tm_mday = 1,           // Day of the month (1-31)
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);

    xtimer_sleep(ALARM_OFFSET_SEC);

    rtc_set_alarm_reltimer(alarm_handler, NULL, ALARM_OFFSET_SEC);

    while (1) {
        pause();
    }

    return 0;
}