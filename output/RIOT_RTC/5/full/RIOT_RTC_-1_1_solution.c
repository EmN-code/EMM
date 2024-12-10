#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

#define ALARM_PERIOD (5 * US_PER_SEC) // 5 seconds

// Alarm callback
void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5; // June (months since January 0-11)
    time.tm_mday = 1; // 1st day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_init();
    rtc_set_time(&time);

    // Setting an alarm for 5 seconds from now
    rtc_set_alarm_offset(ALARM_PERIOD, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(10); // Sleep loop to keep the main function running
    }

    return 0;
}