#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"

void rtc_alarm_cb(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    puts("RTC alarm example");

    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; /* Years since 1900 */
    time.tm_mon = 6 - 1;        /* Months since January */
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    ztimer_sleep(ZTIMER_SEC, 5);

    rtc_cb_t cb;
    cb.callback = rtc_alarm_cb;
    cb.arg = NULL;

    rtc_set_alarm(5, cb);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}