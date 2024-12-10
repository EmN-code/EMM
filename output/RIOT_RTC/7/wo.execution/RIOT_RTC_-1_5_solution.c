#include <timex.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <time.h>
#include <msg.h>
#include <xtimer.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <stdbool.h>
#include <stddef.h>
#include <time_units.h>
#include <unistd.h>
#include <stdlib.h>
#include <evtimer_msg.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info = { .tm_year = 2024 - 1900, .tm_mon = 5, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0 };
    time_t epoch_seconds = mktime(&time_info);
    struct timespec ts = { .tv_sec = epoch_seconds, .tv_nsec = 0 };

    rtc_init();
    rtc_set_time(&ts);

    rtc_alarm_t alarm;
    time_t alarm_seconds = epoch_seconds + 5;
    struct tm *alarm_time_info = localtime(&alarm_seconds);
    rtc_timestamp_to_alarm(alarm_time_info, &alarm);

    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Waiting for alarm...\n");
    }

    return 0;
}