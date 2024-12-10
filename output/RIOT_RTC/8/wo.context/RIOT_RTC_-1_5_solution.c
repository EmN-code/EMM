#include <xtimer.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/gpio.h>

void rtc_alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();
    
    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;             // 0-based index (5 = June)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set the alarm to trigger after 5 seconds
    xtimer_sleep(5);
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}