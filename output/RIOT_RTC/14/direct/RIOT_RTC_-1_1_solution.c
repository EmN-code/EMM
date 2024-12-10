#include <ztimer/ztimer.h>
#include <stdio.h>
#include <xtimer.h>
#include <rtc.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_t rtc_dev;
    struct tm time;
    rtc_alarm_t alarm;

    /* Set the date to June 1, 2024 */
    time.tm_year = 2024;
    time.tm_mon = 6;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&rtc_dev, &time);

    /* Set the alarm to trigger after 5 seconds */
    rtc_get_time(&rtc_dev, &time);
    time.tm_sec += 5;
    mktime(&time);  // Normalize the time structure
    alarm.time = time;
    alarm.callback = rtc_alarm_cb;
    rtc_set_alarm(&rtc_dev, &alarm);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep to reduce busy-waiting
    }

    return 0;
}