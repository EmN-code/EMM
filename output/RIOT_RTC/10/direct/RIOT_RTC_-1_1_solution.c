#include <stdio.h>
#include <time.h>
#include "ztimer.h"

void rtc_alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the date to June 1, 2024
    struct tm date = {0};
    date.tm_year = 2024 - 1900;  // Years since 1900
    date.tm_mon = 5;             // 0-based month, so 5 is June
    date.tm_mday = 1;            // Day of the month
    
    // Calculate the time in seconds since epoch
    time_t epoch_time = mktime(&date);

    struct timespec alarm_time;
    ztimer_t alarm_timer;
    
    // Get current time
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);

    // Setting the alarm 5 seconds after the current time
    alarm_time.tv_sec = current_time.tv_sec + 5;
    alarm_time.tv_nsec = current_time.tv_nsec;

    // Configure ztimer
    ztimer_sleep(ZTIMER_SEC, 5);
    rtc_alarm_handler();

    return 0;
}