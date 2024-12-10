#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <xtimer.h>

#define ALARM_INTERVAL_SEC 5
#define RTC_YEAR 2024
#define RTC_MONTH 6
#define RTC_DAY 1

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time_struct = {0};
    time_struct.tm_year = RTC_YEAR - 1900; // tm_year is year since 1900
    time_struct.tm_mon = RTC_MONTH - 1;    // tm_mon is months since January
    time_struct.tm_mday = RTC_DAY;

    if (rtc_init() != 0) {
        puts("RTC initialization failed!");
        return 1;
    }

    if (rtc_set_time(&time_struct) != 0) {
        puts("Failed to set RTC time!");
        return 1;
    }

    if (rtc_set_alarm_reltimer(ALARM_INTERVAL_SEC, alarm_callback, NULL) != 0) {
        puts("Failed to set RTC alarm!");
        return 1;
    }

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}
