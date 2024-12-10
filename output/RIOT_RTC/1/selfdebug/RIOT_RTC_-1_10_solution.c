#include <stdio.h>
#include <ztimer.h>
#include <event/callback.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Setting up RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;    // June (months since January 0-11)
    time.tm_mday = 1;   // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set alarm for 5 seconds later
    ztimer_t ztimer_alarm;
    ztimer_alarm.callback = alarm_callback;
    ztimer_alarm.arg = NULL;
    ztimer_alarm.interval = 5 * 1000000; // converting seconds to microseconds
    ztimer_set_callback(ZTIMER_USEC, &ztimer_alarm, alarm_callback, NULL);

    // Simulate waiting for alarm (in a real app this would be handled by the system)
    ztimer_sleep(ZTIMER_USEC, ztimer_alarm.interval);

    return 0;
}