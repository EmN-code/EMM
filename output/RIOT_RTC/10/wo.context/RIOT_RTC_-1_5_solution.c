#include <stdio.h>
#include <periph/rtc.h>

void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    rtc_alarm_t alarm;
    rtc_alarm_cb_t alarm_cb = rtc_alarm_callback;  // Declare the variable for the callback

    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // 0-based index, so 5 is June
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting time\n");
        return -1;
    }

    alarm.time = time;
    alarm.time.tm_sec += 5;  // Set alarm 5 seconds from current time
    alarm.callback = alarm_cb;
    alarm.arg = NULL;  // Optional, no argument needed for callback

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Error setting alarm\n");
        return -1;
    }

    while (1) {
        // Main loop, other tasks can be processed here
        // The alarm callback will be triggered when the alarm goes off
    }

    return 0;
}
