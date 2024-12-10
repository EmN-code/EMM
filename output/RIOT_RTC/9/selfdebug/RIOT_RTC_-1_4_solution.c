#include <stdio.h>  
#include <string.h> 
#include <sys/types.h> 
#include <time.h>
#include <xtimer.h>

void set_rtc(time_t timestamp) {
    printf("RTC set to: %s", asctime(localtime(&timestamp)));
}

void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the date to June 1, 2024
    struct tm rtc_time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, 0-11, where 0 = January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    time_t rtc_timestamp = mktime(&rtc_time);
    set_rtc(rtc_timestamp);

    // Set an alarm to trigger after 5 seconds
    xtimer_sleep(5);
    alarm_callback();

    return 0;
}