#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;  // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_init();

    // Set the current date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Years since 1900
    timeinfo.tm_mon = 6 - 1;        // Months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time(&timeinfo);

    // Set an alarm to trigger after 5 seconds
    rtc_get_time(&timeinfo);
    timeinfo.tm_sec += 5;
    mktime(&timeinfo);  // Normalize the time structure
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}