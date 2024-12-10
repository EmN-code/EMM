#include <stdio.h>
#include <ztimer.h>
#include <time.h>
#include <periph/rtc.h>

#define ALARM_DELAY (5U)

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_t alarm_time;

    // Set RTC time to June 1, 2024
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon  = 6 - 1;       // tm_mon is 0-based
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;
    rtc_set_time(&time);

    // Get current RTC time
    rtc_get_time(&alarm_time);

    // Calculate alarm time
    alarm_time.second += ALARM_DELAY;
    ztimer_set(&alarm_callback, NULL, ALARM_DELAY);

    while (1) {
        ztimer_loop();
    }

    return 0;
}
