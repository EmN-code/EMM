#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "mbox.h"
#include "timex.h"
#include "msg.h"
#include "stdatomic.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "thread.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int set_rtc_alarm(void) {
    rtc_time_t time;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_day = 1;
    time.tm_mon = 5;  // June (0-indexed month, so 5 for June)
    time.tm_year = 2024 - 1900; // Years since 1900 for tm_year

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time.\n");
        return -1;
    }

    // Set alarm for 5 seconds later
    ztimer_sleep(ZTIMER_MSEC, 5000);
    alarm_callback();

    return 0;
}

int main(void) {
    printf("RTC Alarm Example\n");
    if (set_rtc_alarm() == 0) {
        printf("Alarm set for 5 seconds later.\n");
    } else {
        printf("Failed to set alarm.\n");
    }
    return 0;
}