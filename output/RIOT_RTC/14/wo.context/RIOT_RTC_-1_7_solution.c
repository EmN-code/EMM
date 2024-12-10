#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "ztimer.h"

void alarm_handler(void *arg) {
    (void)arg; 
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC time to June 1, 2024, 00:00:00
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 6 - 1;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    time_t t = mktime(&timeinfo);
    rtc_set_time(t);

    printf("Current RTC time set to: %s\n", ctime(&t));

    // Set an RTC alarm for 5 seconds in the future
    ztimer_ticks32_t now = ztimer_now(ZTIMER_SEC);
    ztimer_alarm_t alarm;
    ztimer_alarm_init(&alarm, alarm_handler, NULL);
    ztimer_alarm_set(&alarm, now + 5);

    // Keep the program running
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}