#include <stdio.h>
#include <event.h>
#include <cpu_conf.h>
#include <inttypes.h>

#define ALARM_INTERVAL (5 * US_PER_SEC)

void alarm_callback(void *arg) {
    (void)arg;  // Cast to void to indicate 'arg' is unused
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month, 0 - jan
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + ALARM_INTERVAL;
    alarm.cb = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm, 0);

    return 0;
}