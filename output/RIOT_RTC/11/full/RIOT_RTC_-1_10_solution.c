#include "ztimer.h"
#include "xtimer.h"
#include "stdint.h"
#include "periph/rtc.h"
#include "stdio.h"

// Forward declarations
void rtc_alarm_handler(void *arg);

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month, 0-11
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    printf("Current RTC time set to: %s\n", asctime(&time));

    // Setting the alarm
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, rtc_alarm_handler, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}

void rtc_alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}