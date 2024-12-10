#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <kernel_defines.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    /* Set the current date: June 1, 2024 */
    struct tm time = {
        .tm_year = 2024 - 1900,  // year since 1900
        .tm_mon = 6 - 1,         // month, 0-indexed
        .tm_mday = 1,            // day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_init();
    rtc_set_time(&time);

    /* Set an alarm to trigger 5 seconds later */
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;

    rtc_alarm_t alarm = {
        .time = &alarm_time,
        .callback = alarm_callback,
        .callback_arg = NULL
    };

    rtc_set_alarm(&alarm);

    return 0;
}