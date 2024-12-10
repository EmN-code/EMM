#include <stdio.h>
#include <time.h>
#include <ztimer.h>    // Include RIOT's ztimer for delayed execution

static void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC time structure
    struct tm rtc_time;
    rtc_time.tm_year = 2024 - 1900; // Year since 1900
    rtc_time.tm_mon = 5; // June (0 indexed, so 5 is June)
    rtc_time.tm_mday = 1;
    rtc_time.tm_hour = 0;
    rtc_time.tm_min = 0;
    rtc_time.tm_sec = 0;

    // Set the RTC with the specified time
    rtc_set_time(&rtc_time);

    // Set a timer to trigger the alarm after 5 seconds
    ztimer_t timer;
    timer.callback = alarm_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 5000);

    // Main loop does nothing, just wait for the alarm
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}