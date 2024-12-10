#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define ALARM_SECONDS 5

void alarm_handler(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time_struct = {
        .tm_year = 2024 - 1900, // tm_year = current year - 1900
        .tm_mon = 5,            // 0-based index, so 5 = June
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize the RTC
    rtc_init();

    // Set time to the RTC
    time_t current_time = mktime(&time_struct);
    rtc_set_time(current_time);

    // Set an alarm for 5 seconds from now
    current_time += ALARM_SECONDS;
    struct tm *alarm_tm = gmtime(&current_time);

    rtc_alarm_t alarm;
    rtc_timestamp_to_alarm(mktime(alarm_tm), &alarm);
    rtc_set_alarm(&alarm, alarm_handler, NULL);

    while (1) {
        xtimer_sleep(1); // wait until the alarm triggers
    }

    return 0;
}