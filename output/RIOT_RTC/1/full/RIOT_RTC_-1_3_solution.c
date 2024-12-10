#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_t r;
    r.year = 2024;
    r.month = 6;
    r.day = 1;
    r.hour = 0;
    r.minute = 0;
    r.second = 0;

    // Initialize the RTC
    if (rtc_init() != 0) {
        puts("RTC initialization failed!");
        return 1;
    }

    // Set the current date and time
    if (rtc_set_time(r) != 0) {
        puts("Failed to set RTC time!");
        return 1;
    }

    // Set an alarm for 5 seconds in the future
    r.second += 5;
    if (rtc_set_alarm(r, alarm_callback) != 0) {
        puts("Failed to set RTC alarm!");
        return 1;
    }

    puts("RTC initialized and alarm set.");

    while (1) {
        // Main loop
    }

    return 0;
}