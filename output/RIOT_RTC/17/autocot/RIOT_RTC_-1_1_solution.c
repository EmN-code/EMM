#include <stdio.h>  
#include <time.h>

void rtc_alarm_cb(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm alarm_time = {0};
    // Setting date to June 1, 2024
    alarm_time.tm_year = 2024 - 1900;  // years since 1900
    alarm_time.tm_mon = 5;  // months since January (0-11)
    alarm_time.tm_mday = 1;  // day of the month
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 0;

    // Initialize the RTC to the specified time
    rtc_set(&alarm_time);

    // Set alarm 5 seconds after the set time
    time_t seconds = mktime(&alarm_time) + 5;
    struct tm *alarm_time_ptr = localtime(&seconds);
    rtc_set_alarm(alarm_time_ptr, rtc_alarm_cb, NULL);

    // The RTC will now trigger rtc_alarm_cb after 5 seconds
    return 0;
}