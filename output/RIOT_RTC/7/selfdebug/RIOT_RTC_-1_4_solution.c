#include <ztimer.h>
#include <stdio.h>
#include <rtc.h>

void alarm_handler(void *arg){
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();
    
    // Initialize time structure
    struct tm time_info = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 6 - 1,         // Month, 0 - jan
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Convert to RTC time
    rtc_set_time(tm_mktime(&time_info));

    // Set alarm for 5 seconds from now
    ztimer_t timer;
    ztimer_set(ZTIMER_SEC, &timer, 5, alarm_handler, NULL);

    // Keep main thread alive to respond to the alarm
    while(1) {
        ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}