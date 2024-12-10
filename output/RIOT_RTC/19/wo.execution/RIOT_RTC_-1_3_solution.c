#include "thread.h"
#include "time_units.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "stdbool.h"
#include "stdatomic.h"

#include "periph/rtc.h"  // Specific to ESP32 RTC peripheral usage

#define ALARM_OFFSET_SEC   (5U)

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC init failed!\n");
        return 1;
    }

    // Set RTC time to June 1, 2024, 00:00:00
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Years since 1900
        .tm_mon = 5,            // Months since January [0-11]
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(mktime(&timeinfo)) != 0) {
        printf("Failed to set RTC time!\n");
        return 1;
    }

    // Set RTC alarm
    rtc_alarm_t alarm;
    if (rtc_get_time(&alarm.time) != 0) {
        printf("Failed to get RTC time!\n");
        return 1;
    }
    alarm.time.tm_sec += ALARM_OFFSET_SEC;
    mktime(&alarm.time);
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm!\n");
        return 1;
    }

    // Wait indefinitely for the alarm to trigger
    while (1) {
        sleep(1);
    }

    return 0;
}