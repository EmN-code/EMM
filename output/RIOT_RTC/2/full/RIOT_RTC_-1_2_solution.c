#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "time.h"
#include "periph/rtc.h"
#include "periph/rtc_alarm.h"

void alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Set current date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Months since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();  // Initialize the Real-Time Clock
    rtc_set_time(&time);  // Set the RTC time

    // Set alarm for 5 seconds in the future
    alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_alarm_t alarm;
    alarm.time = alarm_time;
    alarm.cb = alarm_handler;  // Set callback function
    alarm.arg = NULL;
    alarm.flags = RTC_ALARM_MATCH_SEC;
    rtc_set_alarm(&alarm);  // Set alarm with callback

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for a while to simulate doing other tasks
    }
    return 0;
}