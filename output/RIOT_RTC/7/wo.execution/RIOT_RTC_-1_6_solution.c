#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mbox.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

void alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    rtc_alarm_t alarm;

    // Set the date and time to June 1, 2024
    struct tm now = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 6 - 1,        // June
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set the RTC time
    rtc_init();
    rtc_time_set(&now);

    // Set the alarm to trigger 5 seconds from now
    rtc_time_get(&alarm_time);
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);

    alarm.time = alarm_time;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm);

    // Busy loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}