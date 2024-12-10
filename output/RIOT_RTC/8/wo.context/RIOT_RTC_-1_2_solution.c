#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>  // For timers, RTC isn't directly available
#include <time.h>

void set_rtc_time(struct tm new_time) {
    // Convert struct tm to time_t and then to system time
    time_t t = mktime(&new_time);
    struct timeval tv = { .tv_sec = t };

    // Set the RTC clock to this new time
    settimeofday(&tv, NULL);
}

void rtc_alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set RTC to June 1, 2024
    struct tm new_time = {
        .tm_year = 2024 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    set_rtc_time(new_time);

    // Simulated alarm using xtimer
    xtimer_sleep(5); // Sleep for 5 seconds
    rtc_alarm_handler(); // Trigger the alarm function

    return 0;
}