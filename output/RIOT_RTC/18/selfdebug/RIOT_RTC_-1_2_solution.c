#include <ztimer.h>
#include <stdio.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    /* Initialize RTC */
    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    /* Set the current time to June 1, 2024, 00:00:00 */
    struct tm time = {
        .tm_year = 2024 - 1900,  /* Year since 1900 */
        .tm_mon = 5,             /* Month 0-11 */
        .tm_mday = 1,            /* Day 1-31 */
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) < 0) {
        printf("Setting RTC time failed\n");
        return -1;
    }

    /* Set an alarm for 5 seconds from now */
    if (rtc_set_alarm_relative(5) < 0) {
        printf("Setting RTC alarm failed\n");
        return -1;
    }

    /* Define the alarm callback */
    rtc_set_alarm_cb(alarm_callback, NULL);

    while (1) {
        /* Enter sleep mode */
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}