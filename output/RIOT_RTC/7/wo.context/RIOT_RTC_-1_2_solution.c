#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg);
void rtc_set_time(struct tm *time);
void rtc_init(void);

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5; // 0-based month (June = 5)
    time.tm_mday = 1; // 1st day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    rtc_set_time(&time);

    // Set an RTC alarm for 5 seconds later
    rtc_set_alarm(&time, alarm_callback, NULL);
    
    // Busy-wait for the alarm (in a real application, use event loops or sleep)
    xtimer_sleep(6);

    return 0;
}