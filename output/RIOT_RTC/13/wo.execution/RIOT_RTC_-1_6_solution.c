#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "unistd.h"
#include "xtimer.h"
#include "mbox.h"
#include "msg.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define ALARM_DELAY 5 // Alarm delay in seconds

void alarm_callback(void) {
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize RTC	tm tm_info;
    tm_info.tm_year = 2024 - 1900;
    tm_info.tm_mon = 5;  // June (0-based, so 5 = June)
    tm_info.tm_mday = 1; // 1st day of the month
    tm_info.tm_hour = 0;
    tm_info.tm_min = 0;
    tm_info.tm_sec = 0;
    time_t rawtime = mktime(&tm_info);
    struct rtc_time current_time;
    gmtime_r(&rawtime, &current_time);

    // Initialize the RTC with current time
    rtc_init();
    rtc_set_time(&current_time);

    // Set up the alarm 5 seconds from now
    struct rtc_alarm alarm;
    alarm.time = current_time;
    alarm.time.tm_sec += ALARM_DELAY;
    rtc_set_alarm(&alarm, alarm_callback);

    // Wait indefinitely to allow the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}