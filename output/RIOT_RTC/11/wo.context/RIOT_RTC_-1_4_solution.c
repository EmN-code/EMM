#include <stdio.h>
#include <time.h>

void rtc_alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time = {
        .tm_year = 124,  // Year since 1900, so 2024 is 124
        .tm_mon = 5,     // June (0-based index, January is 0)
        .tm_mday = 1,    // 1st of June
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };

    time_t future_time = mktime(&date_time) + 5; // Set future time to 5 seconds from set date

    while (time(NULL) < future_time) {
        // Wait until the future time is reached
    }

    rtc_alarm_handler(); // Trigger the alarm

    return 0;
}
