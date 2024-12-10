#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include "util.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/gpio.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    rtc_t dev = RTC_DEV(0);
    rtc_init(dev);

    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // 0-based, 5=June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(dev, &time) != 0) {
        puts("Error setting RTC time");
        return 1;
    }

    puts("Waiting for 5 seconds before setting the alarm...");
    xtimer_sleep(5);

    // Set the alarm for 5 seconds from current time
    time.tm_sec += 5;
    mktime(&time);  // Normalize time structure

    if (rtc_set_alarm(dev, &time, alarm_callback, NULL) != 0) {
        puts("Error setting RTC alarm");
        return 1;
    }

    puts("Alarm set. Waiting...");
    while (1) {
        // Idle loop
    }

    return 0;
}