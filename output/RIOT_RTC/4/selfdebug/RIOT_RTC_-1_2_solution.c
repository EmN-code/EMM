#include "ztimer.h"
#include "periph/rtc.h"
#include <stdio.h>

void alarm_cb(void *arg) {
    (void)arg;  // To prevent unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();
    
    // Set date to June 1, 2024
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,           // June (0-based, 0 = January)
        .tm_mday = 1,           // 1st day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0,
    };
    rtc_set_time(&time);

    // Set alarm to trigger in 5 seconds
    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + 5;
    alarm.callback = alarm_cb;
    alarm.arg = NULL;
    rtc_set_alarm(&alarm);

    // Keep the main thread active
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}