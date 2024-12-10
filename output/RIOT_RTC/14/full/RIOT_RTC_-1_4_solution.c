#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <MQTTClient.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info = {
        .tm_year = 2024 - 1900, // years since 1900
        .tm_mon = 6 - 1,  // zero-based month
        .tm_mday = 1,    
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0 
    };

    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5; // Set the alarm for 5 seconds later

    // Initialize RTC
    while (rtc_init() != 0) {
        printf("RTC initialization failed, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Set RTC time
    while (rtc_set_time(&time_info) != 0) {
        printf("Setting RTC time failed, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Set RTC alarm
    while (rtc_set_alarm(NULL, &alarm_time, alarm_callback, NULL) != 0) {
        printf("Setting RTC alarm failed, retrying...\n");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Idle loop
    while (1) {
        // Continue doing other tasks
        ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}