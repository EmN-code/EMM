#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include <stdbool.h>
#include <stdio.h>

#define ALARM_DURATION_S 5

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t alarm_cb;
    int rc = -1;

    // Set the desired date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;             // Month, 0 = Jan
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    while (1) {
        rc = rtc_init();
        if (rc != 0) {
            printf("RTC initialization failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC initialized successfully.\n");
            break;
        }
    }

    // Set a simulated RTC time
    while (1) {
        rc = rtc_time_set(time);
        if (rc != 0) {
            printf("Setting RTC time failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC time set successfully.\n");
            break;
        }
    }

    // Set up an alarm after ALARM_DURATION_S
    while (1) {
        device_alarm.time = time + ALARM_DURATION_S;
        alarm_cb.callback = alarm_callback;
        rc = rtc_alarm_set(&device_alarm);
        if (rc != 0) {
            printf("Setting RTC alarm failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC alarm set successfully.\n");
            break;
        }
    }

    return 0;
}
