#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <thread.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <event/thread.h>

// RTC time structure
typedef struct {
    int tm_sec;   // seconds
    int tm_min;   // minutes
    int tm_hour;  // hours
    int tm_mday;  // day of the month
    int tm_mon;   // month since January - [0,11]
    int tm_year;  // years since 1900
} rtc_time_t;

// Mock function to set RTC time
int mock_rtc_set_time(rtc_time_t *time) {
    // Normally, this would configure the hardware RTC
    printf("RTC time set: %04d-%02d-%02d %02d:%02d:%02d\n",
           time->tm_year + 1900,
           time->tm_mon + 1,
           time->tm_mday,
           time->tm_hour,
           time->tm_min,
           time->tm_sec);
    return 0;  // success
}

// Mock function to set an RTC alarm
int mock_rtc_set_alarm(int seconds_ahead, void (*callback)(void)) {
    // Normally, this would configure the hardware RTC alarm
    printf("Alarm set for %d seconds from now.\n", seconds_ahead);
    ztimer_sleep(ZTIMER_SEC, seconds_ahead);
    callback();
    return 0;  // success
}

// Callback to be called when alarm triggers
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm Setup\n");

    // Set the date to June 1, 2024
    rtc_time_t current_time = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 0,
        .tm_mday = 1,
        .tm_mon = 5,  // June (0-based, so 5=June)
        .tm_year = 124  // 2024 (since 1900)
    };

    mock_rtc_set_time(&current_time);

    // Set an alarm to trigger 5 seconds from now
    mock_rtc_set_alarm(5, alarm_callback);

    return 0;
}