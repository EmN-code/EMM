#include <xtimer.h>   // Include RTC-related features, xtimer
#include <stdio.h>
#include <time.h>      // Use of struct tm and time functions

#define ALARM_DELAY (5U * US_PER_SEC)  // Alarm delay of 5 seconds

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    puts("RTC Alarm Example");

    struct tm rtc_time;
    rtc_time.tm_year = 2024 - 1900;  // Year since 1900
    rtc_time.tm_mon = 5;             // June, zero-based
    rtc_time.tm_mday = 1;            // 1st day
    rtc_time.tm_hour = 0;            // 00:00:00
    rtc_time.tm_min = 0;
    rtc_time.tm_sec = 0;
    time_t epoch_time = mktime(&rtc_time);  // Convert to time_t epoch

    // Simulating setting the RTC time
    printf("RTC time set to: %s", ctime(&epoch_time));

    // Use xtimer to simulate RTC alarm
    xtimer_usleep(ALARM_DELAY);
    alarm_callback(NULL);

    return 0;
}