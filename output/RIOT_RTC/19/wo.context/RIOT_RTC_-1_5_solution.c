#include <stdio.h>
#include <time.h>
#include <xtimer.h>

void rtc_set_alarm(struct tm *alarm_time) {
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", alarm_time);
    printf("RTC set to: %s\n", buffer);
}

void rtc_check_alarm(struct tm *alarm_time) {
    while (1) {
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);

        if (current_time->tm_year == alarm_time->tm_year &&
            current_time->tm_mon == alarm_time->tm_mon &&
            current_time->tm_mday == alarm_time->tm_mday &&
            current_time->tm_hour == alarm_time->tm_hour &&
            current_time->tm_min == alarm_time->tm_min &&
            current_time->tm_sec == alarm_time->tm_sec) {
            printf("Alarm triggered!\n");
            break;
        }

        xtimer_sleep(1);
    }
}

int main(void) {
    struct tm set_time = {
        .tm_year = 2024 - 1900, // tm_year is year since 1900
        .tm_mon = 5,           // tm_mon in range 0-11, June is 5
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    time_t current_time_t = mktime(&set_time);
    struct tm *alarm_time = localtime(&current_time_t);
    alarm_time->tm_sec += 5;

    rtc_set_alarm(alarm_time);
    rtc_check_alarm(alarm_time);

    return 0;
}