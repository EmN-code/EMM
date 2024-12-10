#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the current time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 5;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;
    time_t t_of_day = mktime(&timeinfo);
    // In a real application, set the RTC to t_of_day here

    // Set the alarm to trigger 5 seconds from now
    ztimer_t timer;
    timer.callback = alarm_callback;
    timer.interval = ZTIMER_USEC_PER_SEC * 5;
    timer.arg = NULL;
    ztimer_set(ZTIMER_USEC, &timer, ZTIMER_USEC_PER_SEC * 5);

    // Keep the main thread alive to listen for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}