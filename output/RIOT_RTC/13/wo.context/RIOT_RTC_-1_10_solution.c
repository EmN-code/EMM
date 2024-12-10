#include <stdio.h>  // For standard input/output functions
#include <string.h> // Required for memset
#include <xtimer.h> // For timer functions
#include <periph/rtc.h> // For RTC functions

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_get_time(&time);

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // Month 0-11
    time.tm_mday = 1; // Day of the month
    rtc_set_time(&time);

    // Set the alarm 5 seconds from now
    xtimer_sleep(5); // Wait to simulate RTC delay
    rtc_get_time(&time); // Update time structure with current time
    time.tm_sec += 5; // Add 5 seconds
    rtc_set_alarm(&time, rtc_alarm_cb, NULL);

    // Keep the main thread running to wait for alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}