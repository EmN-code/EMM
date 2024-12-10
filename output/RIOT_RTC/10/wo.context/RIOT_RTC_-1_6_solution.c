#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set RTC time
    rtc_set_time(&time);

    // Configure an alarm for 5 seconds later
    alarm.time = 5;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    // Set the alarm
    rtc_set_alarm(&alarm);

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}