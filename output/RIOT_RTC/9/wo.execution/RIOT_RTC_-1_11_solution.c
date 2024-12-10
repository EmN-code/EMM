#include "xtimer.h"
#include "stdio.h"
#include "periph/rtc.h"

void rtc_alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Initializing RTC\n");
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);
    printf("Time set to: %s\n", asctime(&time));

    // Set alarm to trigger after 5 seconds
    rtc_set_alarm_relative(5, rtc_alarm_handler);

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}