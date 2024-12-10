#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Initializing RTC with alarm on an ESP32\n");

    struct tm time;
    struct tm alarm_time;
    rtc_t rtc;

    rtc_init();

    // Set the current date and time to June 1, 2024, 00:00:00
    time.tm_year = 124;  // years since 1900
    time.tm_mon = 5;     // months since January [0-11]
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set the alarm time to 5 seconds after the current time
    rtc_set_time(&alarm_time);

    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    printf("Alarm is set and the program is now running\n");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}