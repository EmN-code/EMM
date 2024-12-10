#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon  = 5,  // 0-based month (June)
        .tm_mday = 1,  // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };
    
    if (rtc_init() != 0) {
        puts("RTC initialization failed");
        return 1;
    }

    if (rtc_set_time(&time) != 0) {
        puts("Setting RTC time failed");
        return 1;
    }

    // Set alarm for 5 seconds in the future
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        puts("Setting RTC alarm failed");
        return 1;
    }

    puts("Waiting for alarm...");
    
    while (1) { /* Keep running to maintain the RTC alarm */ }

    return 0;
}