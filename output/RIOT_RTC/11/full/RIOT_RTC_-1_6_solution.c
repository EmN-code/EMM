#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    while(rtc_init() != 0) {
        printf("RTC initialization failed, retrying...\n");
        xtimer_sleep(1);
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    while (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time, retrying...\n");
        xtimer_sleep(1);
    }

    // Set an alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    while (rtc_set_alarm(&alarm_time, alarm_callback) != 0) {
        printf("Failed to set RTC alarm, retrying...\n");
        xtimer_sleep(1);
    }

    printf("RTC alarm set; waiting for it to trigger...\n");

    // Keep the program running to allow the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}