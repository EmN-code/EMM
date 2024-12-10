#include <xtimer.h>
#include <stdio.h>
#include <sched.h>
#include <rtc.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_t dev = RTC_DEV(0);
    struct tm time = {
        .tm_year = 2024 - 1900,   // tm_year is year since 1900
        .tm_mon = 5,              // 0-based, so 5 means June
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize the RTC
    if (rtc_init(dev) < 0) {
        puts("RTC initialization failed");
        return 1;
    }

    // Set the RTC time
    if (rtc_set_time(dev, &time) < 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Schedule an alarm in 5 seconds
    xtimer_sleep(5);
    alarm_callback(NULL);

    return 0;
}