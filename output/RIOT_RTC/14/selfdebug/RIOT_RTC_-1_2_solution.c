#include <xtimer.h>  // Include for xtimer_sleep
#include <stdio.h>  // Include for printf
#include <periph/rtc.h>  // Include for RTC functionalities

void rtc_alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_set_time(0, 0, 8, 1, 6, 2024); // Set RTC to 00:00:08, 1st June 2024
    struct tm alarm_time;
    rtc_get_time(&alarm_time); // Get current set time
    
    // Set alarm to current time + 5 seconds
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the tm structure
    
    rtc_set_alarm(&alarm_time, rtc_alarm_callback);

    // Sleep to wait for alarm
    xtimer_sleep(10);

    return 0;
}