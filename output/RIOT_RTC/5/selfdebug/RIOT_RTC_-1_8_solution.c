#include <stdio.h>
#include <time.h>
#include <periph/rtc.h>

void rtc_alarm_cb(void *arg) {
    (void)arg; // Suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return 1;
    }

    // Set RTC time to June 1, 2024
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 6 - 1;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);

    // Set alarm to trigger 5 seconds from set time
    rtc_alarm_t alarm;
    alarm.time.tm_year = timeinfo.tm_year;
    alarm.time.tm_mon = timeinfo.tm_mon;
    alarm.time.tm_mday = timeinfo.tm_mday;
    alarm.time.tm_hour = timeinfo.tm_hour;
    alarm.time.tm_min = timeinfo.tm_min;
    alarm.time.tm_sec = timeinfo.tm_sec + 5;
    alarm.callback = rtc_alarm_cb;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for T + 5 seconds\n");

    // Main loop does nothing, waits for alarm
    while (1) {
        // Implement any additional functionality if required
    }

    return 0;
}
