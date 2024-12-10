#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"

// RTC alarm callback
void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Current RTC time setup
    struct tm time_struct;
    time_struct.tm_year = 2024 - 1900; // Year since 1900
    time_struct.tm_mon = 6 - 1;        // Month 0-11 (0 = January)
    time_struct.tm_mday = 1;           // Day of the month
    time_struct.tm_hour = 0;
    time_struct.tm_min = 0;
    time_struct.tm_sec = 0;

    // Set up RTC 
    rtc_set_time(&time_struct);

    // Alarm setup time: 5 seconds in the future
    ztimer_sleep(ZTIMER_SEC, 5);

    // Set RTC alarm 5 seconds from the current time
    rtc_alarm_t alarm;
    alarm.time.tm_sec = 5;
    rtc_set_alarm(&alarm, rtc_alarm_callback, NULL);

    printf("RTC is set to: 1st June 2024. Waiting for alarm...\n");

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}