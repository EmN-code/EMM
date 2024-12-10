#include <stdio.h>
#include <event/callback.h>
#include <cpu_conf.h>
#include <container.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC alarm for 2024-06-01\n");

    // Initialize the timer
    ztimer_init(ZTIMER_MSEC, &timer, timer_callback, NULL);

    // Set the RTC to June 1, 2024
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // tm_year is year since 1900
    timeinfo.tm_mon = 5;             // tm_mon is months since January
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    time_t time = mktime(&timeinfo);
    struct timeval now = { .tv_sec = time };
    settimeofday(&now, NULL);

    // Set an alarm for 5 seconds from now
    ztimer_set(ZTIMER_MSEC, &timer, 5000);

    // Main loop
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}