#include <stdio.h>
#include <time.h>
#include "periph_cpuf.h"
#include "periph_conf.h"
#include "periph_cpu.h"

void rtc_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    // Set date to June 1, 2024, 00:00:00
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 5; // tm_mon is 0-based (0 = January)
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);

    // Set alarm for 5 seconds later
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;

    rtc_alarm_set(&alarm_time, rtc_callback, NULL);

    while (1) {
        // Main loop does nothing, waiting for alarm to trigger
        __asm__ volatile("nop");
    }

    return 0;
}