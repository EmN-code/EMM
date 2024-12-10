#include <stdio.h>
#include <riot/rtc.h>
#include <kernel_types.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_t rtc;
    rtc_time_t time;

    // Set date to June 1, 2024
    time.tm_year = 2024; // Year
    time.tm_mon = 6;     // Month
    time.tm_mday = 1;    // Day
    time.tm_hour = 0;    // Hour
    time.tm_min = 0;     // Minute
    time.tm_sec = 0;     // Second

    rtc_init(&rtc, RTC_TYPE_INTERNAL);
    if (rtc_now(&rtc, &time) != 0) {
        printf("Failed to get current RTC time\n");
        return 1;
    }

    // Set time to RTC
    if (rtc_set_time(&rtc, &time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm to 5 seconds in the future
    rtc_time_t alarm_time = time;
    alarm_time.tm_sec += 5;  // Add 5 seconds for the alarm

    if (rtc_set_alarm(&rtc, &alarm_time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}
