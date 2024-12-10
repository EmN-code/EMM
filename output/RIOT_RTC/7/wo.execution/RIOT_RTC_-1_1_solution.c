#include <stdio.h>
#include <time.h>
#include <xtimer.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    (void)arg;  // Suppress unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_init();
    struct tm time_info;

    // Set date to June 1, 2024
    time_info.tm_year = 2024 - 1900;  // Years since 1900
    time_info.tm_mon = 5;             // Months since January (0-11)
    time_info.tm_mday = 1;            // Day of the month (1-31)
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    rtc_set_time(&time_info);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}