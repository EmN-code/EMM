#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <time.h>
#include <mutex.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t current_time;
    rtc_set(current_time, rtc_localtime2unixtime((time_t){.tm_year = 2024 - 1900, .tm_mon = 5, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0, .tm_isdst = -1}));

    rtc_alarm_t alarm;
    alarm.time = current_time + 5;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds later.\n");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}