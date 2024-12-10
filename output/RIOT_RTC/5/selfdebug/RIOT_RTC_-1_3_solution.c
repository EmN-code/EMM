#include <stdio.h>
#include <stdlib.h>
#include "periph_cpu.h"
#include "periph_conf.h"
#include "periph/rtc.h"

#define RTC_ALARM_SECONDS 5

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where January is 0
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time_info) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    if (rtc_alarm_set(&time_info, RTC_ALARM_SECONDS, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm.\n");
        return 1;
    }

    printf("RTC date and alarm set. Waiting for alarm...\n");

    while(1) {
        // Idle loop
    }

    return 0;
}