#include <stdio.h>
#include "xtimer.h"
#include "thread_config.h"
#include "periph_cpu.h"
#include "periph_conf.h"

void rtc_alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();
    
    // Set RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 6 - 1;         // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Calculate alarm time (5 seconds later)
    time_t alarm_time = rtc_get_time() + 5;

    // Set RTC alarm
    rtc_set_alarm(alarm_time, rtc_alarm_callback);

    // Keep the main thread alive
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}