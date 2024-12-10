#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

static void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Month, starting from 0 (June is 5)
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set RTC alarm 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}