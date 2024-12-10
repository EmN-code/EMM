#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();
    
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5; // tm_mon is months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time to June 1, 2024
    rtc_set_time(&time, RTC_FORMAT_CALENDAR);

    // Set an alarm for 5 seconds from now
    rtc_set_alarm_reltimer("rtc0", 5 * RTC_HZ, alarm_callback, NULL);

    while (1) {
        // Just sleep and let the alarm trigger within the callback
        __WFI(); // Wait For Interrupt
    }

    return 0;
}