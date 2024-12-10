#include <stdio.h>
#include <time.h>
#include "rtc_utils.h"

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date;
    rtc_alarm_cb_t cb = alarm_callback;
    void *arg = NULL;

    date.tm_year = 2024 - 1900;  // Years since 1900
    date.tm_mon = 5;             // 0-based, June
    date.tm_mday = 1;            // Day of the month
    date.tm_hour = 0;            // Midnight
    date.tm_min = 0;
    date.tm_sec = 0;

    rtc_init();  // Initialize the RTC
    rtc_set_alarm(&date, cb, arg);  
    rtc_poweron();

    while (1) {
        // Loop forever while waiting for the alarm
    }

    return 0;
}