#include <stdio.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_alarm_cb_t cb = alarm_callback;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // years since 1900
    timeinfo.tm_mon = 6 - 1; // months since January (0-11)
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time!\n");
        return 1;
    }

    // Configure alarm to trigger in 5 seconds
    timeinfo.tm_sec +=5;
    if (rtc_set_alarm(&timeinfo, cb, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
        return 1;
    }

    // Keep the program running to wait for the alarm
    while (1) {
        // You can add additional functionality here
    }

    return 0;
}