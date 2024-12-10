#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>

void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5; // 0 based, 5 for June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    rtc_init(); 

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    struct tm alarm_time = time;
    alarm_time.tm_sec += 5; // Set alarm for 5 seconds later

    while (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm. Retrying...\n");
        xtimer_sleep(1);
    }

    printf("RTC alarm set for 5 seconds later\n");

    // Enter a loop waiting for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}