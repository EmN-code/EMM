#include <stdio.h>
#include <xtimer.h>
#include <time.h>

#define ALARM_DELAY 5

void rtc_alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    struct tm *result_time;

    // Set initial time to June 1, 2024, 00:00:00
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set the RTC with defined time
    rtc_set_time(&timeinfo);

    // Setup RTC alarm for specified delay
    time_t alarm_time = mktime(&timeinfo) + ALARM_DELAY;
    result_time = localtime(&alarm_time);
    rtc_set_alarm(result_time, rtc_alarm_handler);

    while (1) {
        // Main loop can perform other tasks
        xtimer_sleep(1);
    }
    return 0;
}

void rtc_set_time(struct tm *timeinfo) {
    // Dummy function to simulate RTC set time
    printf("RTC set to: %s\n", asctime(timeinfo));
}

void rtc_set_alarm(struct tm *alarm_time, void (*cb)(void *)) {
    // Dummy function to simulate RTC alarm set
    printf("Alarm set for: %s\n", asctime(alarm_time));
    // Simulate alarm after delay
    xtimer_usleep(ALARM_DELAY * 1000000);
    cb(NULL);
}