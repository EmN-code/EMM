#include <stdio.h>
#include <xtimer.h>
#include <ztimer.h>
#include <periph_rtc.h>

#define ALARM_SECONDS (5U)

static void alarm_triggered(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_time_t rtc_time;
    int res;

    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // 0-based month
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_time.time = mktime(&time);

    // Initialize RTC
    if ((res = rtc_init()) != RTC_OK) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set RTC time
    if ((res = rtc_set_time(&rtc_time)) != RTC_OK) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Read back the time and print it
    if ((res = rtc_get_time(&rtc_time)) != RTC_OK) {
        printf("Failed to get RTC time\n");
        return 1;
    }
    printf("Time is set to: %s", ctime(&rtc_time.time));

    // Set an alarm after 5 seconds
    rtc_time_t alarm_time;
    alarm_time.time = rtc_time.time + ALARM_SECONDS;

    rtc_cb_t alarm_cb;
    alarm_cb.callback = alarm_triggered;
    alarm_cb.arg = NULL;

    if ((res = rtc_set_alarm(&alarm_time, &alarm_cb)) != RTC_OK) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Wait for the alarm to trigger
    while (1) {
        xtimer_sleep(ALARM_SECONDS + 1); // Just sleep, alarm will trigger the callback
    }

    return 0;
}