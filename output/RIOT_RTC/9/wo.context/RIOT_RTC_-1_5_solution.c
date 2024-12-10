#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm timeinfo = {
        .tm_year = 124, // 2024 since tm_year is years since 1900
        .tm_mon = 5,    // June (0-based index, 0 = January)
        .tm_mday = 1,   // 1st day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&timeinfo);

    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5; // set alarm 5 seconds into the future

    rtc_alarm_t alarm;
    alarm.time = mktime(&alarm_time);
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    rtc_set_alarm(&alarm, alarm_callback, NULL);
    
    // Busy wait loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}