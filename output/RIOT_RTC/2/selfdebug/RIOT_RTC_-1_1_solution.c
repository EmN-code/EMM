#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

// Callback function for the RTC alarm
void alarm_cb(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm for 5 seconds from now
    struct tm alarm_time = time;
    xtimer_sleep(5);  // wait for 5 seconds
    // Trigger the alarm 5 seconds after the current time
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_cb, NULL);

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}