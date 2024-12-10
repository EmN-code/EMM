#include <time.h>
#include <sys/types.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;

    // Set the date to June 1, 2024
    alarm_time.tm_year = 2024 - 1900; // Note: Year since 1900
    alarm_time.tm_mon = 5;  // Month, where 0 = Jan
    alarm_time.tm_mday = 1; // Day of the month
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 5; // Set the alarm to trigger after 5 seconds

    // Set the alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    return 0;
}