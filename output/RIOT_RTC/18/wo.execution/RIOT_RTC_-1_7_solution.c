#include "xtimer.h"
#include "ztimer.h"
#include "periph/rtc.h"
#include "stdio.h"

void set_rtc_alarm(void) {
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // 0-based month
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
    }

    // Set alarm
    xtimer_sleep(5);  // Sleep for 5 seconds

    // Alarm action
    printf("Alarm triggered!\n");
}

int main(void) {
    puts("Setting RTC alarm\n");
    set_rtc_alarm();
    return 0;
}