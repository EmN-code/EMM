#include <rtc.h>
#include <stdio.h>
#include <xtimer.h>

static int triggered = 0;

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
    triggered = 1;
}

int main(void) {
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    printf("Current time set to: %s\n", asctime(&time));

    xtimer_sleep(1); // ensure the time is set

    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5; // Set alarm for 5 seconds later
    mktime(&alarm_time);     // Normalize the time structure

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    printf("Alarm set for: %s\n", asctime(&alarm_time));

    // Wait for the alarm to be triggered
    while (!triggered) {
        xtimer_sleep(1);
    }

    return 0;
}