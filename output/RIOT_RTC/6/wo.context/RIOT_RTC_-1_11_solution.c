#include <xtimer.h>
#include <stdio.h>
#include <time.h>

#define ALARM_PERIOD_SEC 5

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // 0-based month, so 5 means June
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    
    time_t timestamp = mktime(&date_time);
    struct timeval tv = { .tv_sec = timestamp, .tv_usec = 0 };
    
    settimeofday(&tv, NULL);

    // Set the RTC alarm to trigger after ALARM_PERIOD_SEC
    xtimer_sleep(ALARM_PERIOD_SEC);
    alarm_callback(NULL);

    return 0;
}