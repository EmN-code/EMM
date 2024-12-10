#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void rtc_alarm_callback(void *arg){
    (void)arg; // Unused
    puts("Alarm triggered!");
}

int main(void){
    // Initialize RTC
    rtc_init();

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set an alarm 5 seconds from now
    xtimer_sleep(5);

    if (rtc_set_alarm(&time, rtc_alarm_callback, NULL) != 0) {
        puts("Failed to set alarm");
        return 1;
    }

    // Keep the main function running to ensure alarm callback can be executed
    while(1){
        xtimer_sleep(1);
    }

    return 0;
}