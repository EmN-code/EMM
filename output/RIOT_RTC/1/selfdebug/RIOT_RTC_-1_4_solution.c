#include "xtimer.h"
#include "stdio.h"
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1; // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time.\n");
        return 1;
    }

    // Set an alarm for 5 seconds in the future
    xtimer_sleep(5);
    rtc_set_alarm(rtc_alarm_cb, NULL);

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}