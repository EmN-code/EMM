#include <stdio.h>
#include "xtimer.h"
#include "rtc.h"

void alarm_cb(struct tm *time, void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t alarm_callback;
    
    // Set the current time to June 1, 2024
    time.tm_year = 2024 - 1900; // struct tm is years since 1900
    time.tm_mon = 5; // June is 5 (0-indexed)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    // Initialize RTC with set time
    rtc_init();
    rtc_set_time(&time);
    
    // Set the alarm 5 seconds from now
    xtimer_sleep(5); // Sleep for 5 seconds before setting alarm for demonstration
    rtc_get_time(&time); // Get the current time after 5 seconds
    time.tm_sec += 5; // Set alarm 5 seconds from now

    // If overflow in seconds, handle it
    if (time.tm_sec >= 60) {
        time.tm_sec -= 60;
        time.tm_min += 1;
    }

    // Callback function for alarm
    alarm_callback = alarm_cb;
    
    // Set the alarm
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Keep running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}