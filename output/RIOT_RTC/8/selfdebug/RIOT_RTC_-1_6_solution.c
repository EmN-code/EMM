#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Define and set the time to June 1, 2024, 00:00:00
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where January = 0
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&timeinfo);

    // Schedule an alarm for 5 seconds from the current time
    uint16_t ms;
    rtc_get_time_ms(NULL, &ms); // Get current milliseconds

    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;  // Set the alarm for 5 seconds later
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    return 0;
}