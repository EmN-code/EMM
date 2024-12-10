#include "xtimer_set.h"
#include "stdio.h"
#include "rtc_get_alarm.h"
#include "rtc_get_time.h"
#include "rtc_set_alarm.h"
#include "rtc_set_time.h"

void alarm_callback(void *arg) {
    (void)arg;  // Argument not used
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month (0-11)
        .tm_mday = 1,           // Day of the month
    };
    
    rtc_set_time(&time);

    // Setting alarm for 5 seconds in the future
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5; // Add 5 seconds
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}