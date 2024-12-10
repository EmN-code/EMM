#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    rtc_get_time(&time);
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_mday = 1;
    time.tm_mon = 5;  // Note: Month in struct tm is 0-indexed, so June is 5
    time.tm_year = 2024 - 1900; // Year since 1900
    rtc_set_time(&time);

    xtimer_sleep(5);  // Sleep for 5 seconds before setting the alarm

    rtc_set_alarm(&time, alarm_callback, NULL);
    while (1) {
        // Main loop can perform other tasks or put the device to sleep
        xtimer_sleep(1000);  // Sleep for 1000 seconds or some other logic
    }

    return 0;
}