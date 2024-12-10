#include <ztimer.h>
#include <time.h>
#include <stdio.h>
#include <periph/rtc.h>

static void alarm_callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info = {0};
    time_info.tm_year = 2024 - 1900;  // Year since 1900
    time_info.tm_mon = 6 - 1;         // Month, 0-based
    time_info.tm_mday = 1;            // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return 1;
    }

    rtc_set_time(mktime(&time_info));

    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 5000, alarm_callback, NULL);

    while (1) {
        // Keep thread alive so timer can fire
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}