#include <ztimer/ztimer.h>
#include <stdio.h>
#include <event/callback.h>

// Callback function for the timer
void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC and set date to June 1, 2024
    struct tm time_info;
    time_info.tm_year = 2024 - 1900;  // Year since 1900
    time_info.tm_mon = 5;             // Month (0-based, so 5 represents June)
    time_info.tm_mday = 1;            // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    time_t t = mktime(&time_info);
    rtc_set_time(t);
    
    // Set timer for 5 seconds from now
    ztimer_t alarm_timer;
    ztimer_set(ZTIMER_SEC, &alarm_timer, 5, alarm_callback, NULL);
    
    // Keeping the main alive to wait for the alarm
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}