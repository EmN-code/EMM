#include "periph/rtc.h"
#include "xtimer.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set RTC to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // Years since 1900
        .tm_mon = 6 - 1,         // Months since January
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);

    // Set an alarm 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}