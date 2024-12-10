#include "rtc_alarm_cb_t"
#include "rtc.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // years since 1900
    timeinfo.tm_mon = 6 - 1;         // months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time(&timeinfo);

    struct tm alarm_time;
    alarm_time.tm_year = timeinfo.tm_year;
    alarm_time.tm_mon = timeinfo.tm_mon;
    alarm_time.tm_mday = timeinfo.tm_mday;
    alarm_time.tm_hour = timeinfo.tm_hour;
    alarm_time.tm_min = timeinfo.tm_min;
    alarm_time.tm_sec = 5;  // Set alarm for 5 seconds

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        /* Infinite loop to keep the program running */
    }
    return 0;
}