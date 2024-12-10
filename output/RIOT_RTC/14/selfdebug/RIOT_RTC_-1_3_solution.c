#include <xtimer.h>  // Provides timer functionalities
#include <stdio.h>    // For printf functions
#include <time.h>     // For struct tm and time functions

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;        // Months since January (0-11)
    timeinfo.tm_mday = 1;           // Day of the month (1-31)
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Assume rtc_set_time is a proper function available
    // rtc_set_time(&timeinfo);

    // Wait for 5 seconds using xtimer, then trigger the alarm.
    xtimer_sleep(5);

    // Trigger the alarm
    alarm_callback(NULL);

    return 0;
}
