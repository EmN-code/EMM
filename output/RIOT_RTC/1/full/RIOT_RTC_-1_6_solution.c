#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <assert.h>

#define ALARM_SECONDS 5

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    int res;
    struct tm time;
    struct tm alarm_time;

    // Set the current date and time
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    rtc_init();

    // Set RTC time
    res = rtc_time_set(&time);
    assert(res == 0);

    // Set the alarm time
    alarm_time = time;
    alarm_time.tm_sec += ALARM_SECONDS;

    // Set the alarm
    res = rtc_alarm_set(&alarm_time, alarm_callback, NULL);
    assert(res == 0);

    // Main thread loop
    while (1) {
        xtimer_sleep(1000); // Sleep for a second and wait for the alarm callback
    }

    return 0;
}