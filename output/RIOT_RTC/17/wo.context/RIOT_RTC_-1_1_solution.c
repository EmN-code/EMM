#include <stdio.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    (void)arg; // Unused variable
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month, 0 - jan
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC set. Waiting for alarm...\n");

    // Waiting loop to keep the program alive
    while (1) {}

    return 0;
}