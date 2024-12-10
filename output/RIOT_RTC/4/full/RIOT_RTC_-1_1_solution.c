#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;  // Remove unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_init();

    // Set the date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 5;  // June is month 5 in 'struct tm'
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);

    // Set an alarm for 5 seconds from the current time
    timeinfo.tm_sec += 5;
    rtc_set_alarm(&timeinfo, alarm_callback, NULL);

    // Busy loop to keep the program running to wait for the alarm
    while(1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}