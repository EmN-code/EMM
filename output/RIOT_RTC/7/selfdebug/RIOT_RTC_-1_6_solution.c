#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <rtc.h>

#define ALARM_INTERVAL_SEC (5U)

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // 0-based month
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Create a timer
    ztimer_t alarm_timer;
    ztimer_set_handler(ZTIMER_USEC, &alarm_timer, alarm_callback, NULL);
    ztimer_set(ZTIMER_SEC, &alarm_timer, ALARM_INTERVAL_SEC);

    printf("RTC alarm set for 5 seconds from now.\n");

    // Busy sleep, waiting for the alarm to trigger
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}