#include "time.h"
#include "periph/rtc.h"
#include "periph_cpu.h"
#include <stdio.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {0};

    // Set the date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Month, 0-based in struct tm
    timeinfo.tm_mday = 1;          // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time_tm(&timeinfo);

    // Set the alarm to trigger 5 seconds from now
    rtc_set_alarm(alarm_callback, NULL, 5);

    while (1) {
        // Busy wait to keep the application running as the RTC operates.
    }

    return 0;
}